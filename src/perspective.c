/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   perspective.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 13:41:20 by vkozlov           #+#    #+#             */
/*   Updated: 2019/05/14 00:15:07 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void			do_perspective(t_wall *w, int width, int height)
{
	w->scale1.x = HFOV / w->t1.y * width;
	w->scale1.y = VFOV / w->t1.y * height;
	w->x1 = width / 2 - (int)(w->t1.x * w->scale1.x);
	w->scale2.x = HFOV / w->t2.y * width;
	w->scale2.y = VFOV / w->t2.y * height;
	w->x2 = width / 2 - (int)(w->t2.x * w->scale2.x);
}

t_vertex		reverse_perspective(t_main *m, int x, int y, float height)
{
	t_vertex	r;
	float		rtx;
	float		rty;
	float		dist;

	r.y = height * VFOV * m->sdl.win_h / ((m->sdl.win_h / 2.0 - y) -
									m->map.player.pitch * VFOV * m->sdl.win_h);
	r.x = r.y * (m->sdl.win_w / 2.0 - x) / (HFOV * m->sdl.win_w);
	rtx = r.y * m->map.player.anglecos + r.x * m->map.player.anglesin;
	rty = r.y * m->map.player.anglesin - r.x * m->map.player.anglecos;
	r.x = rtx + m->map.player.position.x;
	r.y = rty + m->map.player.position.y;
	dist = (r.x - m->map.player.position.x) * (r.x - m->map.player.position.x)
	+ (r.y - m->map.player.position.y) * (r.y - m->map.player.position.y);
	dist = sqrtf(dist);
	m->map.ligntness = dist * m->map.player.darkness * 6;
	return (r);
}
