/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_threads.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:01:04 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/16 20:02:58 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_threads(t_data *data)
{
	int		i;
	t_coder	*coder;

	i = -1;
	while (++i < data->n_coders)
	{
		coder = &data->coders[i];
		if (pthread_create(&coder->thr, NULL, c_routine, coder) != 0)
			return (fprintf(stderr, "Failed to create monitor thread\n"), 1);
	}
	if (pthread_create(&data->monitor_thr, NULL, m_routine, data) != 0)
		return (fprintf(stderr, "Failed to create monitor thread\n"), 1);
	return (0);
}