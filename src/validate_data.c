/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:50:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/13 11:36:36 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	validate_data(t_data *data)
{
	if (!data->log_mutex)
		return (fprintf(stderr, "Log mutex init error.\n"), false);
	if (!&data->state_mutex)
		return (fprintf(stderr, "State mutex init error.\n"), false);
	if (!data->dongles)
		return (fprintf(stderr, "Dongles init error.\n"), false);
	if (!data->coders)
		return (fprints(stderr, "Coders init error.\n"), false);
	return (true);
}
