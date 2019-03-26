/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:24:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:28:46 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		draw_local_wall(t_main *m, t_wall *wall, t_renderer *r, int x)
{
	int ya;
	int yb;

	ya = (x - wall->x1) * (wall->y2[0] - wall->y1[0]) /
										(wall->x2 - wall->x1) + wall->y1[0];
	yb = (x - wall->x1) * (wall->y2[1] - wall->y1[1]) /
										(wall->x2 - wall->x1) + wall->y1[1];
	wall->cya = clampf(ya, r->top_limit[x], r->bottom_limit[x]);
	wall->cyb = clampf(yb, r->top_limit[x], r->bottom_limit[x]);
	draw_line(&m->sdl.img, &(t_vline){ x, r->top_limit[x],
									wall->cya, 0, 0x003399, 0 });
	draw_line(&m->sdl.img, &(t_vline){ x, wall->cyb,
							r->bottom_limit[x], 0, 0x009933, 0 });
	if (wall->neighbor < 0)
		draw_line(&m->sdl.img, &(t_vline){ x, wall->cya,
			wall->cyb, 0, x == wall->x1 || x == wall->x2 ? 0 : 0x444444, 0 });
}

static void		draw_neighbor_wall(t_main *m, t_wall *wall,
												t_renderer *r, int x)
{
	int nya;
	int nyb;

	nya = (x - wall->x1) * (wall->neighbor_y2[0] - wall->neighbor_y1[0]) /
								(wall->x2 - wall->x1) + wall->neighbor_y1[0];
	nyb = (x - wall->x1) * (wall->neighbor_y2[1] - wall->neighbor_y1[1]) /
								(wall->x2 - wall->x1) + wall->neighbor_y1[1];
	wall->ncya = clampf(nya, r->top_limit[x], r->bottom_limit[x]);
	wall->ncyb = clampf(nyb, r->top_limit[x], r->bottom_limit[x]);
	draw_line(&m->sdl.img, &(t_vline){ x, wall->cya, wall->ncya - 1, 0, 0x440077, 0 });
	r->top_limit[x] = clampf(maxf(wall->cya, wall->ncya),
												r->top_limit[x], m->sdl.img.h - 1);
	draw_line(&m->sdl.img, &(t_vline){ x, wall->ncyb + 1, wall->cyb,
						0, x == wall->x1 || x == wall->x2 ? 0 : 0x770044, 0 });
	r->bottom_limit[x] = clampf(minf(wall->cyb, wall->ncyb),
														0, r->bottom_limit[x]);
}

void			render_wall(t_main *m, t_renderer *renderer, t_wall *wall,
											t_render_item const *current_sector)
{
	int		beginx;
	int		endx;
	int		x;

	beginx = maxf(wall->x1, current_sector->limit_x_left);
	endx = minf(wall->x2, current_sector->limit_x_right);
	x = beginx - 1;
	while (++x <= endx)
	{
		draw_local_wall(m, wall, renderer, x);
		if (wall->neighbor >= 0)
			draw_neighbor_wall(m, wall, renderer, x);
	}
	if (wall->neighbor >= 0 && endx >= beginx)
	{
		*renderer->head = (t_render_item){ wall->neighbor, beginx, endx };
		if (++renderer->head == renderer->queue + MaxQueue)
			renderer->head = renderer->queue;
	}
}

void			draw_screen(t_main *m)
{
	t_renderer		r;
	t_render_item	current_sector;

	init_renderer(&r, &m->sdl.img, m->map.number_sectors);
	(*r.head) = (t_render_item){m->map.player.sector_number, 0, m->sdl.img.w - 1};
	++r.head;
	if (r.head == r.queue + MaxQueue)
		r.head = r.queue;
	while (r.head != r.tail)
	{
		current_sector = *r.tail;
		++r.tail;
		if (r.tail == r.queue + MaxQueue)
			r.tail = r.queue;
		if (r.rendered_sectors[current_sector.sectorno] == MaxQueue)
			continue;
		render_sector(m, &r, &current_sector);
		r.rendered_sectors[current_sector.sectorno]++;
	}
	free_renderer(&r);
}
