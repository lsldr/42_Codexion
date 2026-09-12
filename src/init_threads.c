/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:55:03 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 18:59:02 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_threads(t_data *data, coder_routine *routine)
{
	int			i;
	pthread_t	*threads;

	i = -1;
	threads = malloc(sizeof(pthread_t) * data->n_coders);
	while (++i < data->n_coders)
	{
		if (pthread_create(&threads[i], NULL, routine, &data->coders[i]) != 0)
			return (perror("Failed to create thread"));
	}
}