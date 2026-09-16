/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:50:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/16 10:32:57 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_ds(t_data *data)
{
	if (!data->monitor_thr)
		return (fprintf(stderr, "Monitor init error.\n"), false);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles init error.\n"), false);
	if (!data->coders)
		return (fprintf(stderr, "Coders init error.\n"), false);
	return (true);
}
