/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 20:03:50 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 13:11:13 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Coders wait on their cond variable to see
// if the dongle has become available for them.
static void	dongles_timed_wait(t_coder *coder,
	t_dongle *first_dongle, t_dongle *second_dongle)
{
	unsigned long		timed_wait_ms;
	struct timespec		ts;

	timed_wait_ms = get_wait_time_ms(first_dongle, second_dongle);
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
	req.coder = coder;
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
	push_requests(coder);
	while (!is_simulation_over(coder->data))
	{
		pthread_mutex_lock(&coder->c_mutex);
		coder->is_wake = false;
		pthread_mutex_unlock(&coder->c_mutex);
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
		dongles_timed_wait(coder, first_dongle, second_dongle);
	}
	if (is_simulation_over(coder->data))
		return (exit_queues(coder, first_dongle, second_dongle), true);
	return (false);
}

// Update dongle's `in_use` and `free_t` fields,
// then signal wake coders in the queue.
void	release_dongle(t_dongle *dongle, unsigned int cooldown_ms)
{
	short	i;
	short	queue_size;

	i = -1;
	pthread_mutex_lock(&dongle->mutex);
	queue_size = dongle->queue.size;
	dongle->free_t = get_time_ms() + cooldown_ms;
	dongle->in_use = false;
	while (++i < queue_size)
		wake_coder(dongle->queue.items[i].coder);
	pthread_mutex_unlock(&dongle->mutex);
}
