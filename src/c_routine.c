/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:58:39 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 13:17:11 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Handle the single coder case: take the dongle and end up burning out.
static bool	handle_single_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->l_dongle->mutex);
	coder->l_dongle->in_use = true;
	pthread_mutex_unlock(&coder->l_dongle->mutex);
	log_action(coder, "has taken a dongle");
	while (!is_simulation_over(coder->data))
		usleep(1000);
	return (true);
}

// After setting the first and second variables' addresses to the left
// and right dongles of the coder, return true when the coder can't
// take both of them, or just handle the single coder case with a
// burnout when starved without compiles while holding the only dongle.
static bool	take_dongles(t_coder *coder)
{
	unsigned short	first_dongle_index;
	unsigned short	second_dongle_index;

	if (!coder->r_dongle)
		return (handle_single_coder(coder));
	if (coder->l_dongle->id < coder->r_dongle->id)
	{
		first_dongle_index = coder->l_dongle->id - 1;
		second_dongle_index = coder->r_dongle->id - 1;
	}
	else
	{
		first_dongle_index = coder->r_dongle->id - 1;
		second_dongle_index = coder->l_dongle->id - 1;
	}
	if (acquire_dongles(coder,
			&coder->data->dongles[first_dongle_index],
			&coder->data->dongles[second_dongle_index]))
		return (true);
	log_action(coder, "has taken a dongle");
	log_action(coder, "has taken a dongle");
	return (false);
}

// Update the last compile start time after dongles were taken,
// log compilation, sleep with regular checks on simulation end,
// then release dongles and return the true if compilation was interrupted.
static bool	compile_phase(t_coder *coder)
{
	bool	res;

	pthread_mutex_lock(&coder->c_mutex);
	coder->last_cc_t = get_time_ms();
	pthread_mutex_unlock(&coder->c_mutex);
	log_action(coder, "is compiling");
	res = coder_sleep(coder, coder->data->compile_t);
	release_dongle(coder->l_dongle, coder->data->d_cooldown);
	release_dongle(coder->r_dongle, coder->data->d_cooldown);
	return (res);
}

// Do the full cycle of compiles with logs, returning
// true if the coder was interrupted in any one of them.
bool	compile_cycle(t_coder *coder)
{
	if (take_dongles(coder))
		return (true);
	if (compile_phase(coder))
		return (true);
	pthread_mutex_lock(&coder->c_mutex);
	coder->cc_count++;
	pthread_mutex_unlock(&coder->c_mutex);
	log_action(coder, "is debugging");
	if (coder_sleep(coder, coder->data->debug_t))
		return (true);
	log_action(coder, "is refactoring");
	if (coder_sleep(coder, coder->data->refactor_t))
		return (true);
	return (false);
}

// Coder routine: while simulation has not ended, do the
// compile-debug-refactor cycles, and update the the cc count
// accordingly.
void	*c_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->data->req_compiles == 0)
		return (NULL);
	if ((coder->coder_num & 1) == 0)
		usleep(500);
	while (!is_simulation_over(coder->data))
	{
		if (compile_cycle(coder))
			continue ;
		if (coder->data->req_compiles > 0
			&& coder->cc_count >= coder->data->req_compiles)
			break ;
	}
	return (NULL);
}
