/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:50:30 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/19 12:01:16 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Free all the allocated memory and destroy
// pthread_mutex_t and pthread_cond_t variables
void	clean_all(t_data *data)
{
	int	i;

	i = -1;
	if (!data)
		return ;
	if (data->coders)
	{
		while (++i < data->n_coders)
		{
			pthread_mutex_destroy(&data->coders[i].c_mutex);
			pthread_cond_destroy(&data->coders[i].cond);
		}
		free(data->coders);
	}
	i = -1;
	if (data->dongles)
	{
		while (++i < data->n_coders)
			pthread_mutex_destroy(&data->dongles[i].mutex);
		free(data->dongles);
	}
	pthread_mutex_destroy(&data->log_mutex);
	pthread_mutex_destroy(&data->end_mutex);
	pthread_cond_destroy(&data->end_cond);
	free(data);
}
