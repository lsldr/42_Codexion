/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:58:39 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 20:01:49 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	is_simulation_over(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->is_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}

static bool	log_action(t_coder *coder, char *action)
{
	unsigned long	ts;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (coder->data->is_end)
		return (pthread_mutex_unlock(&coder->data->log_mutex), false);
	ts = get_time_ms() - coder->data->start_t;
	printf("%lu %u %s\n", ts, coder->coder_num, action);
	pthread_mutex_unlock(&coder->data->log_mutex);
	return (true);
}

static bool	coder_sleep(t_coder *coder, unsigned int duration_ms)
{
	unsigned long	deadline;

	deadline = get_time_ms() + duration_ms;
	while (!is_simulation_over(coder->data))
	{
		if (get_time_ms() >= deadline)
			return (false);
		usleep(500);
	}
	return (true);
}

bool	compile_cycle(t_coder *coder);

void	*c_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_simulation_over(coder->data))
	{
		if (compile_cycle(coder))
			break ;
		pthread_mutex_lock(&coder->c_mutex);
		coder->cc_count++;
		pthread_mutex_unlock(&coder->cc_count);
		if (coder->data->req_compiles > 0
			&& coder->cc_count >= coder->data->req_compiles)
			break ;
	}
	return (NULL);
}
