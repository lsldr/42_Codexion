/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_routine.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:58:39 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 21:08:37 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	handle_single_coder(t_coder *coder)
{
	if (acquire_dongle(coder, coder->l_dongle))
		return (true);
	log_action(coder, "has taken a dongle");
	while (!is_simulation_over(coder->data))
		usleep(1000);
	return (true);
}

static bool	take_dongles(t_coder *coder, t_dongle **f, t_dongle **s)
{
	if (!coder->r_dongle)
		return (handle_single_coder(coder));
	if (coder->l_dongle->id < coder->r_dongle->id)
	{
		*f = coder->l_dongle;
		*s = coder->r_dongle;
	}
	else
	{
		*f = coder->r_dongle;
		*s = coder->l_dongle;
	}
	if (acquire_dongle(coder, *f))
		return (true);
	log_action(coder, "has taken a dongle");
	if (acquire_dongle(coder, *s))
	{
		release_dongle(*f, coder->data->d_cooldown);
		return (true);
	}
	log_action(coder, "has taken a dongle");
	return (false);
}

static bool	compile_phase(t_coder *coder, t_dongle *f, t_dongle *s)
{
	bool	res;

	pthread_mutex_lock(&coder->c_mutex);
	coder->last_cc_t = get_time_ms();
	pthread_mutex_unlock(&coder->c_mutex);
	log_action(coder, "is compiling");
	res = coder_sleep(coder, coder->data->compile_t);
	release_dongle(f, coder->data->d_cooldown);
	release_dongle(s, coder->data->d_cooldown);
	return (res);
}

bool	compile_cycle(t_coder *coder)
{
	t_dongle	*f;
	t_dongle	*s;

	if (take_dongles(coder, &f, &s))
		return (true);
	if (compile_phase(coder, f, s))
		return (true);
	log_action(coder, "is debugging");
	if (coder_sleep(coder, coder->data->debug_t))
		return (true);
	log_action(coder, "is refactoring");
	if (coder_sleep(coder, coder->data->refactor_t))
		return (true);
	return (false);
}

void	*c_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->coder_num % 2 == 0)
		usleep(500);
	while (!is_simulation_over(coder->data))
	{
		if (compile_cycle(coder))
			break ;
		pthread_mutex_lock(&coder->c_mutex);
		coder->cc_count++;
		pthread_mutex_unlock(&coder->c_mutex);
		if (coder->data->req_compiles > 0
			&& coder->cc_count >= coder->data->req_compiles)
			break ;
	}
	return (NULL);
}
