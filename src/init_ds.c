/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:45:45 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 11:08:04 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders(t_data *data)
{
	int		i;

	i = -1;
	data->coders = malloc(sizeof(t_coder) * data->n_coders);
	if (!data->coders)
		return (fprintf(stderr, "Coders malloc failed.\n"));
	while (++i < data->n_coders)
	{
		data->coders[i].coder_num = i + 1;
		data->coders[i].cc_count = 0;
		data->coders[i].data = data;
		data->coders[i].thr = malloc(sizeof(pthread_t));
		if (!data->coders[i].thr)
			return (fprintf(stderr, "Coder pthread malloc failed\n"));
		data->coders[i].cond = malloc(sizeof(pthread_cond_t));
		if (!data->coders[i].cond)
			return (fprintf(stderr, "Pthread cond malloc failed.\n"));
		pthread_cond_init(data->coders[i].cond, NULL);
		data->coders[i].last_cc_t = 0;
	}
}

void	init_dongles(t_data *data)
{
	int	i;

	i = -1;
	data->dongles = malloc(sizeof(t_dongle) * data->n_coders);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles malloc failed.\n"));
	while (++i < data->n_coders)
	{
		data->dongles[i].mutex = malloc(sizeof(pthread_mutex_t));
		if (!data->dongles[i].mutex)
			return (fprintf(stderr, "Dongle mutex malloc failed.\n"));
		pthread_mutex_init(data->dongles[i].mutex, NULL);
		data->dongles[i].queue = malloc(sizeof(t_pqueue));
		if (!data->dongles[i].queue)
			return (fprintf(stderr, "Priority queue malloc failed.\n"));
	}
}

void	assign_dongles(t_data *data)
{
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

void	set_queues(t_data *data)
{
	t_pqueue		*q;
	unsigned short	i;

	i = data->n_coders;
	if (!data->dongles)
		return ;
	while (--i > 0)
	{
		q = data->dongles[i].queue;
		q->front_coder_num = i + 1;
		q->back_coder_num = (i + 2) % data->n_coders;
	}
	q = data->dongles[i].queue;
	q->front_coder_num = i + 1;
	q->back_coder_num = data->n_coders;
	q = NULL;
}

void	init_ds(t_data *data)
{
	init_coders(data);
	init_dongles(data);
	assign_dongles(data);
	set_queues(data);
}
