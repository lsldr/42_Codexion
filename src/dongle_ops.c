/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 20:03:50 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/19 16:01:53 by asuleime         ###   ########.fr       */
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

// A coder waits on their cond var to see
// if the dongle mutex has been freed.
static void	dongle_wait(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_nsec += 5000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&coder->cond, &dongle->mutex, &ts);
}

// Push into dongle queue and wait until this coder is highest priority
// and dongle is available. Returns true if interrupted by is_end.
bool	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	req;

	req.coder_num = coder->coder_num;
	req.key = get_key(coder);
	req.cond = &coder->cond;
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->queue, req);
	while (!is_simulation_over(coder->data))
	{
		if (heap_peek(&dongle->queue).coder_num == coder->coder_num
			&& !dongle->in_use && get_time_ms() >= dongle->free_t)
			break ;
		dongle_wait(coder, dongle);
	}
	if (is_simulation_over(coder->data))
	{
		heap_remove(&dongle->queue, coder->coder_num);
		pthread_mutex_unlock(&dongle->mutex);
		return (true);
	}
	heap_pop(&dongle->queue);
	dongle->in_use = true;
	pthread_mutex_unlock(&dongle->mutex);
	return (false);
}

// Signal all waiters in queue to re-check their condition
static void	wake_dongle_queue(t_dongle *dongle)
{
	int	i;

	i = -1;
	while (++i < dongle->queue.size)
		pthread_cond_signal(dongle->queue.items[i].cond);
}

// Update dongle's `in_use` and `free_t` fields,
// then signal this to the coders from the queue.
void	release_dongle(t_dongle *dongle, unsigned int cooldown_ms)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->in_use = false;
	dongle->free_t = get_time_ms() + cooldown_ms;
	wake_dongle_queue(dongle);
	pthread_mutex_unlock(&dongle->mutex);
}
