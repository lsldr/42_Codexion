/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 19:40:44 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 13:43:20 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wake_coder(t_coder *c)
{
	pthread_mutex_lock(&c->c_mutex);
	c->is_wake = true;
	pthread_cond_signal(&c->cond);
	pthread_mutex_unlock(&c->c_mutex);
}

// Check if simulation has ended
bool	is_simulation_over(t_data *data)
{
	bool			result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->is_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}

// Print a log message on action
bool	log_action(t_coder *coder, char *action)
{
	unsigned long	ts;

	pthread_mutex_lock(&coder->data->log_mutex);
	pthread_mutex_lock(&coder->data->end_mutex);
	if (coder->data->is_end)
	{
		pthread_mutex_unlock(&coder->data->log_mutex);
		pthread_mutex_unlock(&coder->data->end_mutex);
		return (false);
	}
	pthread_mutex_unlock(&coder->data->end_mutex);
	ts = get_time_ms() - coder->data->start_t;
	printf("%lu %u %s\n", ts, coder->coder_num, action);
	pthread_mutex_unlock(&coder->data->log_mutex);
	return (true);
}

// Sleep for 1 millisecond while checking if simulation ended
bool	coder_sleep(t_coder *coder, unsigned int duration_ms)
{
	unsigned long	deadline;

	deadline = get_time_ms() + duration_ms;
	while (!is_simulation_over(coder->data))
	{
		if (get_time_ms() >= deadline)
			return (false);
		usleep(1000);
	}
	return (true);
}
