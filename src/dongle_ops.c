/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 20:03:50 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/20 17:24:31 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Get key: request timestamp (FIFO) or the deadline to burnout (EDF)
static unsigned long	get_key(t_coder *coder)
{
	if (coder->data->scheduler == FIFO)
		return (get_time_ms());
	return (coder->last_cc_t + coder->data->burnout_t);
}

// Coders wait on their cond variable to see
// if the dongle has become available for them.
// Used timed wait to make sure it does not wait
// too long when the simulation ends.
static void	dongle_wait(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&coder->cond, &dongle->mutex, &ts);
}

void	push_requests(t_coder *coder)
{
	t_request	req;

	req.coder_num = coder->coder_num;
	req.key = get_key(coder);
	req.cond = &coder->cond;
	pthread_mutex_lock(&coder->l_dongle->mutex);
	heap_push(&coder->l_dongle->queue, req);
	pthread_mutex_unlock(&coder->l_dongle->mutex);
	pthread_mutex_lock(&coder->r_dongle->mutex);
	heap_push(&coder->r_dongle->queue, req);
	pthread_mutex_unlock(&coder->r_dongle->mutex);
}

// Push requests into left and right dongles' queues,
// Wait for them to free if 
bool	acquire_dongles(t_coder *coder, t_dongle *l_dongle, t_dongle *r_dongle)
{
	push_requests(coder);
	while (!is_simulation_over(coder->data))
	{
		pthread_mutex_lock(&l_dongle->mutex);
		pthread_mutex_lock(&r_dongle->mutex);
		if (heap_peek(&l_dongle->queue) == coder->coder_num
			&& !l_dongle->in_use && get_time_ms() >= l_dongle->free_t
			&& heap_peek(&r_dongle->queue) == coder->coder_num
			&& !r_dongle->in_use && get_time_ms() >= r_dongle->free_t)
		{
			pop_queues(coder);
			pthread_mutex_unlock(&l_dongle->mutex);
			pthread_mutex_unlock(&r_dongle->mutex);
			break ;
		}
		pthread_mutex_unlock(&l_dongle->mutex);
		pthread_mutex_unlock(&r_dongle->mutex);
		dongle_wait(coder, l_dongle);
		dongle_wait(coder, r_dongle);
	}
	if (is_simulation_over(coder->data))
		return (exit_queues(coder), true);
	return (false);
}

// Update dongle's `in_use` and `free_t` fields,
// then signal this to the first-in-queue coder.
void	release_dongle(t_dongle *dongle, unsigned int cooldown_ms)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->free_t = get_time_ms() + cooldown_ms;
	dongle->in_use = false;
	if (dongle->queue.size > 0)
		pthread_cond_signal(dongle->queue.items[0].cond);
	pthread_mutex_unlock(&dongle->mutex);
}
