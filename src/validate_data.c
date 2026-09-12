/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:50:55 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 18:53:56 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	validate_data(t_data *data)
{
	if (!data->log_mutex || !&data->state_mutex)
		return (false);
	if (!data->dongles || !data->coders)
		return (false);
	return (true);
}
