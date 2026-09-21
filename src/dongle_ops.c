/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 20:03:50 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 11:58:17 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Get key: request timestamp (FIFO) or the deadline to burnout (EDF).
static unsigned long	get_key(t_coder *coder)
{
	if (coder->data->scheduler == FIFO)
		return (get_time_ms());
	return (coder->last_cc_t + coder->data->burnout_t);
}

// Coders wait on their cond variable to see
// if the dongle has become available for them.
static void	dongles_timed_wait(t_coder *coder,
	t_dongle *first_dongle, t_dongle *second_dongle,
	unsigned long timed_wait_ms)
{
	struct timespec		ts;

	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += timed_wait_ms / 1000;
	ts.tv_nsec += (timed_wait_ms % 1000) * 1000000L;
	if (ts.tv_nsec >= 1000000000L)
	{
		ts.tv_sec += 1;
		ts.tv_nsec -= 1000000000L;
	}
	pthread_mutex_unlock(&second_dongle->mutex);
	pthread_mutex_unlock(&first_dongle->mutex);
	pthread_mutex_lock(&coder->c_mutex);
	pthread_cond_timedwait(&coder->cond, &coder->c_mutex, &ts);
	pthread_mutex_unlock(&coder->c_mutex);
}

// Push coder's requests to the two dongles' queues the can take.
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

// Push requests, then check if both can be taken,
// if yes, proceed with the routine for compilation,
// if no, wait for them to become free with timed waits.
bool	acquire_dongles(t_coder *coder,
	t_dongle *first_dongle, t_dongle *second_dongle)
{
	unsigned long	timed_wait_ms;

	timed_wait_ms = 0;
	push_requests(coder);
	while (!is_simulation_over(coder->data))
	{
		pthread_mutex_lock(&first_dongle->mutex);
		pthread_mutex_lock(&second_dongle->mutex);
		if (heap_peek(&first_dongle->queue) == coder->coder_num
			&& !first_dongle->in_use && get_time_ms() >= first_dongle->free_t
			&& heap_peek(&second_dongle->queue) == coder->coder_num
			&& !second_dongle->in_use && get_time_ms() >= second_dongle->free_t)
		{
			pop_queues(coder);
			pthread_mutex_unlock(&second_dongle->mutex);
			pthread_mutex_unlock(&first_dongle->mutex);
			break ;
		}
		timed_wait_ms = get_wait_time_ms(first_dongle, second_dongle);
		dongles_timed_wait(coder, first_dongle, second_dongle, timed_wait_ms);
	}
	if (is_simulation_over(coder->data))
		return (exit_queues(coder, first_dongle, second_dongle), true);
	return (false);
}

// Update dongle's `in_use` and `free_t` fields,
// then signal this to the first-in-queue coder.
void	release_dongle(t_dongle *dongle, unsigned int cooldown_ms)
{
	int		i;
	int		queue_size;

	i = -1;
	pthread_mutex_lock(&dongle->mutex);
	queue_size = dongle->queue.size;
	dongle->free_t = get_time_ms() + cooldown_ms;
	dongle->in_use = false;
	pthread_mutex_unlock(&dongle->mutex);
	while (++i < queue_size)
		pthread_cond_signal(dongle->queue.items[i].cond);
}
