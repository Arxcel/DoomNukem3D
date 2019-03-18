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

float cross_product(t_vertex point_a, t_vertex point_b, t_vertex point_c, t_vertex point_d)
{
    t_vertex ab;
    t_vertex cd;
    float cross;

	ab.x = point_b.x - point_a.x;
    ab.y = point_b.y - point_a.y;
    cd.x = point_d.x - point_c.x;
    cd.y = point_d.y - point_c.y;
    cross = ab.x * cd.y - ab.y * cd.x;
    return cross;
}

float distance(t_vertex point_a, t_vertex point_b)
{
    float d1;
    float d2;

	d1 = point_a.x = point_b.x;
	d2 = point_a.y = point_b.y;

    return (sqrtf(d1 * d1 + d2 * d2));
}

float line_to_point_distance(t_vertex start, t_vertex end, t_vertex point)
{
    float dist;

	dist = cross_product(start, end, start, point) / distance(start, end);
    return fabs(dist);
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
