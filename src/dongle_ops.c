/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 20:03:50 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/20 12:22:43 by asuleime         ###   ########.fr       */
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

// A coder waits on their cond variable to
// see if the dongle mutex has been unlocked.
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
