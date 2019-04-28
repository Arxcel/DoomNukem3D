/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cast_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 14:02:30 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 20:20:01 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static double		ray_segment_intersaction(t_vertex v1,
												t_vertex v2, t_vertex v3)
{
	double dot;
	double t1;
	double t2;

	dot = v_dot(v2, v3);
	if (fabs(dot) < 0.000001)
		return (-1.0);
	t1 = crossf(v2.x, v2.y, v1.x, v1.y) / dot;
	t2 = v_dot(v1, v3) / dot;
	if (t1 >= 0.0 && (t2 >= 0.0 && t2 <= 1.0))
		return (t1);
	return (-1.0);
}

float				calc_distance(t_vector v1, t_vector v2)
{
	float	abx;
	float	aby;
	float	dist;

	abx = (v1.x - v2.x);
	aby = (v1.y - v2.y);
	dist = (float)sqrt(abx * abx + aby * aby);
	return (dist);
}

double				cast_ray_2line(t_vertex ray_origin,
					t_vertex ray_direction, t_vertex point1, t_vertex point2)
{
	t_vertex v1;
	t_vertex v2;
	t_vertex v3;

	v1.x = ray_origin.x - point1.x;
	v1.y = ray_origin.y - point1.y;
	v2.x = point2.x - point1.x;
	v2.y = point2.y - point1.y;
	v3 = (t_vertex){-ray_direction.y, ray_direction.x};
	return (ray_segment_intersaction(v1, v2, v3));
}
