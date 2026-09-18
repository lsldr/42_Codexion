/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 10:06:10 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 21:29:02 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	has_priority(t_request req_a, t_request req_b)
{
	if (req_a.key != req_b.key)
		return (req_a.key < req_b.key);
	return (req_a.coder_num < req_b.coder_num);
}

t_request	heap_peek(t_pqueue *queue)
{
	return (queue->items[0]);
}

void	heap_push(t_pqueue *q, t_request req)
{
	int			i;
	int			parent;
	t_request	tmp;

	i = q->size;
	q->items[i] = req;
	q->size++;
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

t_request	heap_pop(t_pqueue *q)
{
	t_request	top;
	t_request	tmp;
	int			i;
	int			min;

	top = q->items[0];
	q->size--;
	q->items[0] = q->items[q->size];
	i = 0;
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
	return (top);
}

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
			break ;
		}
	}
}