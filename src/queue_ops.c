/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_ops.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 10:06:10 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/17 10:43:44 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	has_priority(t_request req_a, t_request req_b)
{
	if (req_a.key != req_b.key)
		return (req_a.key < req_b.key);
	return (req_a.coder_num < req_b.coder_num);
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

t_request	heap_pop(t_pqueue *q, int index, int left)
{
	t_request	top;
	t_request	tmp;
	int			right;
	int			min;

	top = q->items[0];
	q->size--;
	q->items[0] = q->items[q->size];
	while (q->size > 0)
	{
		min = index;
		right = left + 1;
		if (left < q->size && has_priority(q->items[left], q->items[min]))
			min = left;
		if (right < q->size && has_priority(q->items[right], q->items[min]))
			min = right;
		if (min == index)
			break ;
		tmp = q->items[index];
		q->items[index] = q->items[min];
		q->items[min] = tmp;
		index = min;
	}
	return (top);
}