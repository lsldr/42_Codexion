/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/21 11:46:14 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 11:57:05 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Get current time in milliseconds.
unsigned long	get_time_ms(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	return ((unsigned long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

// Set start time of the simulation in milliseconds,
// set coders' initial last compile time at this value.
void	set_time(t_data *data)
{
	int		i;

	i = -1;
	data->start_t = get_time_ms();
	while (++i < data->n_coders)
		data->coders[i].last_cc_t = data->start_t;
}

unsigned long	get_wait_time_ms(
	t_dongle *first_dongle, t_dongle *second_dongle)
{
	unsigned long	wait_ms;
	unsigned long	now;

	wait_ms = 5;
	now = get_time_ms();
	if (first_dongle->free_t > now
		&& (first_dongle->free_t - now) < wait_ms)
		wait_ms = first_dongle->free_t - now;
	if (second_dongle->free_t > now
		&& (second_dongle->free_t - now) < wait_ms)
		wait_ms = second_dongle->free_t - now;
	if (wait_ms == 0)
		wait_ms = 1;
	return (wait_ms);
}