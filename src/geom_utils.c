/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geom_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:45:42 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:45:56 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <math.h>

float		crossf(float x0, float y0, float x1, float y1)
{
	return (x0 * y1 - x1 * y0);
}

double			v_dot(t_vertex v1, t_vertex v2)
{
	double	res;

	res = v1.x * v2.x + v1.y * v2.y;
	return (res);
}

bool			overlapf(float begin0, float end0, float begin1, float end1)
{
	return (minf(begin0, end0) < maxf(begin1, end1)
				&& minf(begin1, end1) < maxf(begin0, end0));
}

bool			intersect_box(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4)
{
	return (overlapf(v1.x, v2.x, v3.x, v4.x)
								&& overlapf(v1.y, v2.y, v3.y, v4.y));
}

t_vertex	intersect_line(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4)
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

float		point_side(t_vertex p, t_vertex v1, t_vertex v2)
{
	return (crossf(v2.x - v1.x, v2.y - v1.y, p.x - v1.x, p.y - v1.y));
}

