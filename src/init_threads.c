/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:55:03 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/13 16:50:29 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	malloc_logmutex(t_data *data)
{
	data->log_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->log_mutex)
		return (frpintf(stderr, "Logmutex malloc failed.\n");
}

malloc_monitor(t_data *data)
{
	data->monitor = malloc(sizeof(t_monitor));
	init_threads(data);
	if (!data->monitor)
		return (frpintf(stderr, "Monitor malloc failed.\n");
	data->monitor->coders = data->coders;
}

void	init_threads(t_data *data)
{
	int			i;
	pthread_t	*thrs;

	i = -1;
	malloc_logmutex(data);
	malloc_monitor(data);
	thrs = malloc(sizeof(pthread_t) * data->n_coders);
	while (++i <= data->n_coders)
		if (pthread_create(&thrs[i], NULL, routine, (void *)&data->coders[i]))
			return (fprintf(stderr, "Failed to create threads.\n"));
	while (--i > 0)
		data->coders[i].thr = &thrs[i];
	data->monitor.thr = &thrs[i];
}
