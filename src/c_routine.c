/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:58:39 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/17 16:06:43 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*c_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;	
	return ((void *)coder);
}
