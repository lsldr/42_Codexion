/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:50:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/14 11:51:50 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_data(t_data *data)
{
	if (!data->log_mutex)
		return (fprintf(stderr, "Log mutex init error.\n"), false);
	if (!&data->monitor)
		return (fprintf(stderr, "Monitor init error.\n"), false);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles init error.\n"), false);
	if (!data->coders)
		return (fprints(stderr, "Coders init error.\n"), false);
	return (true);
}
