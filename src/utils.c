/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:47:26 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 12:53:16 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "stdio.h"

float		minf(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

float		maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float		clampf(float a, float min, float max)
{
	return (minf(maxf(a, min), max));
}

int			intersects(t_player const *p, t_vertex v1, t_vertex v2)
{
	t_vertex p_old_pos;
	t_vertex p_new_pos;

	p_old_pos = (t_vertex) { p->position.x, p->position.y };
	p_new_pos = (t_vertex) { p->position.x + p->velocity.x,
									p->position.y + p->velocity.y };
	return (intersect_box(p_old_pos, p_new_pos, v1, v2) &&
							point_side(p_new_pos, v1, v2) < 0);
}

t_interp	*init_interp(t_point from, int c, t_point to)
{
	t_interp *i;

	i = (t_interp*)malloc(sizeof(t_interp));
	i->current = to.x + (from.y - 1 - from.x) * (to.y - to.x) / (c - from.x);
	i->step = ((to.y < to.x) ^ (c < from.x)) ? -1 : 1;
	i->target_length = abs(to.y - to.x);
	i->basic_length = abs(c - from.x);
	i->temp = (int)((from.y - 1 - from.x) * abs(to.y - to.x)) % abs(c - from.x);
	return (i);
}

int			interp_next(t_interp *i)
{
	i->temp += i->target_length;
	while (i->temp >= i->basic_length)
	{
		i->current += i->step;
		i->temp -= i->basic_length;
	}
	return (i->current);
}
