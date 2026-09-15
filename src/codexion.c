/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:31:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 12:09:44 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	codexion(t_data *data)
{
	int	i;

	i = -1;
	init_ds(data);
	while (++i < data->n_coders)
		pthread_join(data->coders[i].thr, NULL);
	pthread_join(data->monitor_thr, NULL);
}
