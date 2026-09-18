/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   m_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 11:00:28 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/17 16:33:27 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_all_coders(void)
{

}

void	*m_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)data;
	while (true)
	{
		usleep(1000);
		check_all_coders();
	}
	return (NULL);
}
