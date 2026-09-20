/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:25:15 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/20 17:11:44 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Get the coder num at the top of the queue
unsigned short	heap_peek(t_pqueue *queue)
{
	if (queue->size > 0)
		return (queue->items[0].coder_num);
	return (0);
}

// Check if the request from the first param
// has priority, returning true if so.
bool	has_priority(t_request req_a, t_request req_b)
{
	if (req_a.key != req_b.key)
		return (req_a.key < req_b.key);
	return (req_a.coder_num < req_b.coder_num);
}

void	exit_queues(t_coder *coder)
{
	pthread_mutex_lock(&coder->r_dongle->mutex);
	heap_remove(&coder->r_dongle->queue, coder->coder_num);
	pthread_mutex_unlock(&coder->r_dongle->mutex);
	pthread_mutex_lock(&coder->l_dongle->mutex);
	heap_remove(&coder->l_dongle->queue, coder->coder_num);
	pthread_mutex_unlock(&coder->l_dongle->mutex);
}

void	pop_queues(t_coder *coder)
{
	heap_pop(&coder->r_dongle->queue);
	coder->r_dongle->in_use = true;
	heap_pop(&coder->l_dongle->queue);
	coder->l_dongle->in_use = true;
}
