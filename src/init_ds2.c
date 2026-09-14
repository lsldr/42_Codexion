/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:55:03 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/14 11:34:19 by asuleime         ###   ########.fr       */
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
}

void	init_threads(t_data *data)
{
	int			i;
	pthread_t	*thrs;

	i = -1;
	thrs = malloc(sizeof(pthread_t) * data->n_coders);
	while (++i <= data->n_coders)
		if (pthread_create(&thrs[i], NULL, routine, &data->coders[i]))
			return (fprintf(stderr, "Failed to create threads.\n"));
	while (--i > 0)
		data->coders[i].thr = &thrs[i];
	data->monitor.thr = &thrs[i];
}

void	init_ds2(t_data *data)
{
	init_logmutex(data);
	init_monitor(data);
	init_threads(data);
}
