/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:31:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/20 17:21:27 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Simulation master function. 
// Clean all memory if initialization
// of data structures or threads fails.
// Wait on all the coder threads and the
// monitor thread, then also clean all.
void	codexion(t_data *data)
{
	int	i;

	i = -1;
	if (init_ds(data))
	{
		clean_all(data);
		return ;
	}
	if (init_threads(data))
	{
		clean_all(data);
		return ;
	}
	while (++i < data->n_coders)
		pthread_join(data->coders[i].thr, NULL);
	pthread_join(data->monitor_thr, NULL);
	clean_all(data);
}
