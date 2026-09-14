/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:55:03 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/14 21:25:35 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_logmutex(t_data *data)
{
	data->log_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->log_mutex)
		return (frpintf(stderr, "Logmutex malloc failed.\n");
}

void	init_monitor(t_data *data)
{
	data->monitor = malloc(sizeof(t_monitor));
	if (!data->monitor)
		return (frpintf(stderr, "Monitor malloc failed.\n");
	data->monitor->coders = data->coders;
	data->monitor->is_end = false;
	data->monitor->end_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->monitor->end_mutex)
		return (fprintf(stderr, "End mutex malloc failure.\n"));
}

void	init_threads(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < data->n_coders)
	{
		coder = &data->coders[i];
		pthread_create(&coder->thr, NULL, coder_routine, coder);
	}
	pthread_create(&data->monitor.thr, NULL, monitor_routine, data);
}

void	init_ds2(t_data *data)
{
	init_logmutex(data);
	init_monitor(data);
	init_threads(data);
}
