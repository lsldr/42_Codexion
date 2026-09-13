/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:31:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/13 12:05:14 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	codexion(t_data *data)
{
	int		i;

	i = -1;
	init_coders(data);
	if (!validate_data(data))
		return (fprintf(stderr, "Data initialization failed.\n"));
	init_threads(data);
	while (++i < data->n_coders)
		pthread_join(data->threads[i], NULL);
}
