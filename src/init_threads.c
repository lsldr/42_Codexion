/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:55:03 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/13 12:03:16 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_threads(t_data *data)
{
	int			i;
	pthread_t	*thrs;

	i = -1;
	thrs = malloc(sizeof(pthread_t) * data->n_coders);
	while (++i < data->n_coders)
		if (pthread_create(&thrs[i], NULL, routine, (void *)&data->coders[i]))
			return (fprintf(stderr, "Failed to create threads.\n"));
	data->threads = thrs;
}
