/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:50:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 10:59:00 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_ds(t_data *data)
{
	if (!data->log_mutex)
		return (fprintf(stderr, "Log mutex init error.\n"), false);
	if (!&data->monitor)
		return (fprintf(stderr, "Monitor init error.\n"), false);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles init error.\n"), false);
	if (!data->coders)
		return (fprintf(stderr, "Coders init error.\n"), false);
	return (true);
}
