/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:45:45 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/13 16:53:39 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	malloc_coders(t_data *data)
{
	data->coders = malloc(sizeof(t_coder) * data->n_coders);
	if (!data->coders)
		return ;
}

void	malloc_set_dongles(t_data *data)
{
	int	i;

	i = -1;
	data->dongles = malloc(sizeof(t_dongle) * data->n_coders);
	if (!data->dongles)
		return ;
	while (++i < data->n_coders)
	{
		data->dongles[i].d_mutex = malloc(sizeof(pthread_mutex_t));
		if (!data->dongles[i].d_mutex)
			return (fprintf(stderr, "Dongle mutex malloc failed.\n"));
		pthread_mutex_init(data->dongles[i].d_mutex, NULL);
		data->dongles[i].pqueue = malloc(sizeof(t_pqueue));
		if (!data->dongles[i].pqueue)
			return (fprintf("Priority queue malloc failed.\n"));
	}
}

void	malloc_init_logmutex(t_data *data)
{
	data->log_mutex = malloc(sizeof(pthread_mutex_t));
	if (!data->log_mutex)
		return ;
}

void	assign_dongles(t_data *data)
{
	t_coder	*coder;
	int		i;

	i = -1;
	if (!data->coders || !data->dongles)
		return ;
	while (++i < data->n_coders)
	{
		(data->coders)[i].l_dongle = &(data->dongles[i]);
		(data->coders)[i].r_dongle = &(data->dongles[(i + 1) % data->n_coders]);
	}
}

void	init_coders(t_data *data)
{
	malloc_coders(data);
	malloc_set_dongles(data);
	malloc_init_logmutex(data);
	assign_dongles(data);
}
