/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 12:25:15 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/19 12:37:08 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Get the request at the top of the queue
t_request	heap_peek(t_pqueue *queue)
{
	return (queue->items[0]);
}

// Check if the request from the first param
// has priority, returning true if so.
bool	has_priority(t_request req_a, t_request req_b)
{
	if (req_a.key != req_b.key)
		return (req_a.key < req_b.key);
	return (req_a.coder_num < req_b.coder_num);
}
