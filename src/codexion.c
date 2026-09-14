/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:31:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/14 11:51:35 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	codexion(t_data *data)
{
	int		i;

	i = -1;
	init_ds(data);
	init_ds2(data);
	if (!check_ds(data))
		return (fprintf(stderr, "Data initialization failed.\n"));
	while (++i <= data->n_coders)
		pthread_join(data->threads[i], NULL);
}
