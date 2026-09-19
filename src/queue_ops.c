/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 10:06:10 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/19 17:29:14 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// After a new request is added, fix it up if needed
static void	fix_up(t_pqueue *q, int i)
{
	int			parent;
	t_request	tmp;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!has_priority(q->items[i], q->items[parent]))
			break ;
		tmp = q->items[i];
		q->items[i] = q->items[parent];
		q->items[parent] = tmp;
		i = parent;
	}
}

static void	fix_down(t_pqueue *q, int i)
{
	int			min;
	t_request	tmp;

	min = 0;
	while (2 * i + 1 < q->size)
	{
		min = i;
		if (has_priority(q->items[2 * i + 1], q->items[min]))
			min = 2 * i + 1;
		if (2 * i + 2 < q->size
			&& has_priority(q->items[2 * i + 2], q->items[min]))
			min = 2 * i + 2;
		if (min == i)
			break ;
		tmp = q->items[i];
		q->items[i] = q->items[min];
		q->items[min] = tmp;
		i = min;
	}
}

// Push a request to the queue.
// First, put it at the last index,
// then bubble it up in case it has
// priority over the parent request. 
void	heap_push(t_pqueue *q, t_request req)
{
	q->items[q->size] = req;
	q->size++;
	fix_up(q, q->size - 1);
}

// Pop a request from the queue and heapify after.
// "Heapify" means re-ordering with higher priority
// requests sitting above others.
t_request	heap_pop(t_pqueue *q)
{
	t_request	top;

	top = q->items[0];
	q->size--;
	if (q->size > 0)
	{
		q->items[0] = q->items[q->size];
		fix_down(q, 0);
	}
	return (top);
}

// Remove a request identified by coder_num from the queue.
void	heap_remove(t_pqueue *q, unsigned short coder_num)
{
	int	i;

	i = -1;
	while (++i < q->size)
	{
		if (q->items[i].coder_num == coder_num)
		{
			q->size--;
			q->items[i] = q->items[q->size];
			if (i > 0 && has_priority(q->items[i], q->items[(i - 1) / 2]))
				fix_up(q, i);
			else
				fix_down(q, i);
			break ;
		}
	}
}
