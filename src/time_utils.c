/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 19:40:44 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 21:54:06 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

unsigned long	get_time_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ((unsigned long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void	set_time(t_data *data)
{
	int		i;

	i = -1;
	data->start_t = get_time_ms();
	while (++i < data->n_coders)
		data->coders[i].last_cc_t = data->start_t;
}

bool	is_simulation_over(t_data *data)
{
	bool	result;

	pthread_mutex_lock(&data->end_mutex);
	result = data->is_end;
	pthread_mutex_unlock(&data->end_mutex);
	return (result);
}

bool	log_action(t_coder *coder, char *action)
{
	unsigned long	ts;

	pthread_mutex_lock(&coder->data->log_mutex);
	if (coder->data->is_end)
	{
		pthread_mutex_unlock(&coder->data->log_mutex);
		return (false);
	}
	ts = get_time_ms() - coder->data->start_t;
	printf("%lu %u %s\n", ts, coder->coder_num, action);
	pthread_mutex_unlock(&coder->data->log_mutex);
	return (true);
}

bool	coder_sleep(t_coder *coder, unsigned int duration_ms)
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
