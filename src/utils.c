/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:47:26 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:47:29 by vkozlov          ###   ########.fr       */
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

int			intersects(t_player const *p, t_sector const *cur_s,
							int v1, int v2)
{
	int res1;
	int res2;
	float side;
	t_vertex p_old_pos;
	t_vertex p_new_pos;

	p_old_pos =  (t_vertex) { p->position.x, p->position.y };
	p_new_pos = (t_vertex) { p->position.x + p->velocity.x, p->position.y + p->velocity.y };
	printf("pX :%f\n", p_new_pos.x);
	printf("pY :%f\n", p_new_pos.y);
	res1 = intersect_box(p_old_pos, p_new_pos, cur_s->vertices[v1],
												cur_s->vertices[v2]);
	printf("intersect :%d\n", res1);
	side = point_side(p_new_pos, cur_s->vertices[v1],
											cur_s->vertices[v2]);
											res2 = side < 0;
	printf("sidev :%f\n", side);
	printf("side :%d\n", res2);
	// res &= !(p_new_pos.x > cur_s->max.x || p_new_pos.x < cur_s->min.x);
	// res &= !(p_new_pos.y > cur_s->max.y || p_new_pos.y < cur_s->min.y);
	return (res1 && res2);
}
