/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:00:28 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 21:28:58 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Set is_end=True and signal end of simulation to all threads
static void	signal_end(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->end_mutex);
	data->is_end = true;
	pthread_mutex_unlock(&data->end_mutex);
	i = -1;
	while (++i < data->n_coders)
		pthread_cond_broadcast(&data->coders[i].cond);
}

// Check if any coder has burned out. Prints a msg if returning True
static bool	is_any_burnout(t_data *data)
{
	unsigned long	now;
	int				i;

	now = get_time_ms();
	i = -1;
	while (++i < data->n_coders)
	{
		pthread_mutex_lock(&data->coders[i].c_mutex);
		if (data->req_compiles > 0
			&& data->coders[i].cc_count >= data->req_compiles)
		{
			pthread_mutex_unlock(&data->coders[i].c_mutex);
			continue ;
		}
		if (now - data->coders[i].last_cc_t > data->burnout_t)
		{
			pthread_mutex_unlock(&data->coders[i].c_mutex);
			pthread_mutex_lock(&data->log_mutex);
			pthread_mutex_lock(&data->end_mutex);
			data->is_end = true;
			pthread_mutex_unlock(&data->end_mutex);
			printf("%lu %u burned out\n", now - data->start_t,
				data->coders[i].coder_num);
			pthread_mutex_unlock(&data->log_mutex);
			return (true);
		}
		pthread_mutex_unlock(&data->coders[i].c_mutex);
	}
	return (false);
}

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

void	*m_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (true)
	{
		usleep(1000);
		if (data->req_compiles > 0 && are_enough_compiles(data))
		{
			signal_end(data);
			break ;
		}
		if (is_any_burnout(data))
		{
			signal_end(data);
			break ;
		}
	}
	return (NULL);
}
