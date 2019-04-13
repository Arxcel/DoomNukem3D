/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intersections.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 13:05:35 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 13:34:52 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int				intersects(t_player const *p, t_vertex v1, t_vertex v2)
{
	t_vertex p_old_pos;
	t_vertex p_new_pos;

	p_old_pos = (t_vertex) { p->position.x, p->position.y };
	p_new_pos = (t_vertex) { p->position.x + p->velocity.x,
									p->position.y + p->velocity.y };
	return (intersect_box(p_old_pos, p_new_pos, v1, v2) &&
							point_side(p_new_pos, v1, v2) < 0);
}

t_vertex		intersect_line(t_vertex v1, t_vertex v2,
											t_vertex v3, t_vertex v4)
{
	t_vertex result;

	result.x = crossf(crossf(v1.x, v1.y, v2.x, v2.y),
					v1.x - v2.x, crossf(v3.x, v3.y, v4.x, v4.y), v3.x - v4.x) /
					crossf(v1.x - v2.x, v1.y - v2.y, v3.x - v4.x, v3.y - v4.y);
	result.y = crossf(crossf(v1.x, v1.y, v2.x, v2.y), v1.y - v2.y,
					crossf(v3.x, v3.y, v4.x, v4.y), v3.y - v4.y) /
					crossf(v1.x - v2.x, v1.y - v2.y, v3.x - v4.x, v3.y - v4.y);
	return (result);
}
