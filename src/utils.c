/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:47:26 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/31 13:43:03 by vkozlov          ###   ########.fr       */
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
