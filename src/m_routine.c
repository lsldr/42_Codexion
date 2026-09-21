/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:00:28 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 15:11:50 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Set is_end=True
static void	signal_end(t_data *data)
{
	int		i;

	i = -1;
	pthread_mutex_lock(&data->end_mutex);
	data->is_end = true;
	pthread_mutex_unlock(&data->end_mutex);
	while (++i < data->n_coders)
		wake_coder(&data->coders[i]);
}

// Skip the burnout check for a coder if they met the
// required compiles check for otherwise a burnout
// would be logged when the coders were to finish the compiles.
static bool	is_coder_finished(t_data *data, int i)
{
	if (data->req_compiles > 0
		&& data->coders[i].cc_count >= data->req_compiles)
	{
		pthread_mutex_unlock(&data->coders[i].c_mutex);
		return (true);
	}
	return (false);
}

// Check if any coder has burned out.
// Prints a log message if returning true.
static bool	is_any_burnout(t_data *data)
{
	unsigned long	now;
	int				i;

	i = -1;
	while (++i < data->n_coders)
	{
		pthread_mutex_lock(&data->coders[i].c_mutex);
		if (is_coder_finished(data, i))
			continue ;
		now = get_time_ms();
		if (now - data->coders[i].last_cc_t > data->burnout_t)
		{
			pthread_mutex_unlock(&data->coders[i].c_mutex);
			signal_end(data);
			pthread_mutex_lock(&data->log_mutex);
			printf("%lu %u burned out\n", now - data->start_t,
				data->coders[i].coder_num);
			pthread_mutex_unlock(&data->log_mutex);
			return (true);
		}
		pthread_mutex_unlock(&data->coders[i].c_mutex);
	}
	return (false);
}

// Compare each coder's cc_count to check against the required amount.
static bool	are_enough_compiles(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_coders)
	{
		pthread_mutex_lock(&data->coders[i].c_mutex);
		if (data->coders[i].cc_count < data->req_compiles)
		{
			pthread_mutex_unlock(&data->coders[i].c_mutex);
			return (false);
		}
		pthread_mutex_unlock(&data->coders[i].c_mutex);
	}
	return (true);
}

// Monitor routine. Sleep 1000ms in each iteration,
// check if the compiles count requirement is met or
// if there is a burnout. If so, signal end to all threads.
void	*m_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (true)
	{
		if (is_any_burnout(data))
		{
			signal_end(data);
			break ;
		}
		if (data->req_compiles == 0 || are_enough_compiles(data))
		{
			signal_end(data);
			pthread_mutex_lock(&data->log_mutex);
			printf("Each coder has compiled %u times.\n", data->req_compiles);
			pthread_mutex_unlock(&data->log_mutex);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}
