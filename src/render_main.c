/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:24:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 13:50:11 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		draw_local_wall(t_main *m, t_wall *wall, t_renderer *r, int x)
{
	SDL_Surface		*current;
	t_interp		*i;

	current = m->tex.t.textures[wall->solid_id];
	wall->ya = (x - wall->x1) * (wall->y2[0] - wall->y1[0]) /
										(wall->x2 - wall->x1) + wall->y1[0];
	wall->yb = (x - wall->x1) * (wall->y2[1] - wall->y1[1]) /
										(wall->x2 - wall->x1) + wall->y1[1];
	wall->cya = clampf(wall->ya, r->top_limit[x], r->bottom_limit[x]);
	wall->cyb = clampf(wall->yb, r->top_limit[x], r->bottom_limit[x]);
	i = init_interp((t_pt){wall->ya, wall->cya},
									wall->yb, (t_pt){0, current->w});
	if (wall->neighbor < 0)
		draw_line(m, wall, &(t_vline){x, wall->cya,
										wall->cyb, wall->solid_id}, i);
	free(i);
}

static void		draw_neighbor_wall(t_main *m, t_wall *wall,
												t_renderer *r, int x)
{
	SDL_Surface		*cu;
	SDL_Surface		*cl;
	t_interp		*i;

	cu = m->tex.t.textures[wall->upper_id];
	cl = m->tex.t.textures[wall->lower_id];
	wall->nya = (x - wall->x1) * (wall->neighbor_y2[0] - wall->neighbor_y1[0])
	/ (wall->x2 - wall->x1) + wall->neighbor_y1[0];
	wall->nyb = (x - wall->x1) * (wall->neighbor_y2[1] - wall->neighbor_y1[1])
	/ (wall->x2 - wall->x1) + wall->neighbor_y1[1];
	wall->ncya = clampf(wall->nya, r->top_limit[x], r->bottom_limit[x]);
	wall->ncyb = clampf(wall->nyb, r->top_limit[x], r->bottom_limit[x]);
	i = init_interp((t_pt){wall->ya, wall->cya}, wall->yb, (t_pt){0, cu->w});
	draw_line(m, wall, &(t_vline){x, wall->cya,
										wall->ncya - 1, wall->upper_id}, i);
	r->top_limit[x] = clampf(maxf(wall->cya, wall->ncya),
										r->top_limit[x], m->sdl.img.h - 1);
	free(i);
	i = init_interp((t_pt){wall->ya, wall->ncyb + 1},
	wall->yb, (t_pt){0, cl->w});
	draw_line(m, wall, &(t_vline){x, wall->ncyb + 1,
	wall->cyb, wall->lower_id}, i);
	r->bottom_limit[x] = clampf(minf(wall->cyb,
	wall->ncyb), 0, r->bottom_limit[x]);
	free(i);
}

static void		draw_ceil_floor(t_main *m, t_renderer *r, t_wall *w, int x)
{
	int			y;
	t_vertex	t;
	unsigned	tex[2];
	int			*pix;
	SDL_Surface	*current;

	y = r->top_limit[x] - 1;
	while (++y <= r->bottom_limit[x])
	{
		if (y >= w->cya && y <= w->cyb)
		{
			y = w->cyb;
			continue;
		}
		t = reverse_perspective(m, x, y, y < w->cya ? w->ceil : w->floor);
		tex[0] = (t.x * 2048);
		tex[1] = (t.y * 2048);
		current = y < w->cya ? m->tex.t.textures[w->floor_id] :
												m->tex.t.textures[w->ceil_id];
		pix = current->pixels;
		sdl_pixel_put(&m->sdl.img, x, y, pix[tex[0] % current->w +
										(tex[1] % current->h) * current->w]);
	}
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
		wall->txtx = (wall->u0 * ((wall->x2 - x) * renderer->t2.y) + wall->u1 *
		((x - wall->x1) * renderer->t1.y)) / ((wall->x2 - x) * renderer->t2.y +
		(x - wall->x1) * renderer->t1.y);
		draw_ceil_floor(m, renderer, wall, x);
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
	(*r.head) = (t_render_item){m->map.player.sector_number,
												0, m->sdl.img.w - 1};
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
