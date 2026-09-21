/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:45:45 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 12:50:37 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Initialize coder structures, assigning coder numbers
// and doing pthread's initalizations.
// `last_cc_t` to be set to data->start_t before launching
static int	init_coders(t_data *data)
{
	int			i;
	t_coder		*coder;

	i = -1;
	data->coders = malloc(sizeof(t_coder) * data->n_coders);
	if (!data->coders)
		return (fprintf(stderr, "Coders malloc failed.\n"), 1);
	while (++i < data->n_coders)
	{
		coder = &(data->coders[i]);
		coder->coder_num = i + 1;
		coder->cc_count = 0;
		coder->last_cc_t = 0;
		coder->l_dongle = NULL;
		coder->r_dongle = NULL;
		coder->is_wake = false;
		coder->data = data;
		pthread_mutex_init(&(coder->c_mutex), NULL);
		pthread_cond_init(&(coder->cond), NULL);
	}
	return (0);
}

// Called in init_dongles() below, assign each
// coder's right and left dongles, setting the
// right dongle to NULL if n_coders == 1.
static int	assign_dongles(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->n_coders)
	{
		data->coders[i].l_dongle = &data->dongles[i];
		if (data->n_coders > 1)
			data->coders[i].r_dongle = &data->dongles[(i + 1) % data->n_coders];
		else
			data->coders[i].r_dongle = NULL;
	}
	return (0);
}

// Initialize dongle structures and set up
// their links to coders as per the task.
// Also initialize dongles' mutexes.
static int	init_dongles(t_data *data)
{
	int			i;
	t_dongle	*d;

	i = -1;
	data->dongles = malloc(sizeof(t_dongle) * data->n_coders);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles malloc failed.\n"), 1);
	while (++i < data->n_coders)
	{
		d = &data->dongles[i];
		d->id = i + 1;
		d->in_use = false;
		d->free_t = 0;
		d->queue.size = 0;
		pthread_mutex_init(&d->mutex, NULL);
	}
	assign_dongles(data);
	return (0);
}

// Initialize all the data structures
// needed for the simulation.
int	init_ds(t_data *data)
{
	data->start_t = 0;
	data->is_end = false;
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_cond_init(&data->end_cond, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	if (init_coders(data))
		return (1);
	if (init_dongles(data))
		return (2);
	set_time(data);
	return (0);
}
