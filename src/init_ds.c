/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ds.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:45:45 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 12:14:46 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_coders(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = -1;
	data->coders = malloc(sizeof(t_coder) * data->n_coders);
	if (!data->coders)
		return (fprintf(stderr, "Coders malloc failed.\n"));
	while (++i < data->n_coders)
	{
		coder = &(data->coders[i]);
		coder->coder_num = i + 1;
		pthread_mutex_init(&(coder->c_mutex), NULL);
		pthread_cond_init(&(coder->cond), NULL);
		coder->cc_count = 0;
		coder->last_cc_t = 0;
		coder->l_dongle = NULL;
		coder->r_dongle = NULL;
		coder->data = data;
	}
}

void	init_dongles(t_data *data)
{
	int	i;

	i = -1;
	data->dongles = malloc(sizeof(t_dongle) * data->n_coders);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles malloc failed.\n"));
	if (!data->coders)
		return ;
	while (++i < data->n_coders)
	{
		data->coders[i].l_dongle = &(data->dongles[i]);
		data->coders[i].r_dongle = &(data->dongles[(i + 1) % data->n_coders]);
	}
	init_queues(data);
}

void	init_queues(t_data *data)
{
	return ;
}

void	init_threads(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < data->n_coders)
		pthread_create(data->coders[i].thr, NULL, c_routine, &data->coders[i]);
	pthread_create(&data->monitor_thr, NULL, m_routine, data);
}

void	init_ds(t_data *data)
{
	data->start_t = 0;
	data->is_end = false;
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->log_mutex, NULL);
	init_coders(data);
	init_dongles(data);
	if (!check_ds(data))
		return (fprintf(stderr, "Data initialization failed.\n"));
	init_threads(data);
}
