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
	wall->ya = (x - wall->x1) * (wall->y2[0] - wall->y1[0]) /
										(wall->x2 - wall->x1) + wall->y1[0];
	wall->yb = (x - wall->x1) * (wall->y2[1] - wall->y1[1]) /
										(wall->x2 - wall->x1) + wall->y1[1];
	wall->cya = clampf(wall->ya, r->top_limit[x], r->bottom_limit[x]);
	wall->cyb = clampf(wall->yb, r->top_limit[x], r->bottom_limit[x]);
	if (wall->neighbor < 0)
		draw_line2(m, &(t_vline){ x, wall->cya, wall->cyb, 0,0,0,2}, (t_interp)INIT_INTERP(wall->ya, wall->cya, wall->yb, 0,1023), wall->txtx);
}

static void		draw_neighbor_wall(t_main *m, t_wall *wall,
												t_renderer *r, int x)
{
	wall->nya = (x - wall->x1) * (wall->neighbor_y2[0] - wall->neighbor_y1[0]) /
								(wall->x2 - wall->x1) + wall->neighbor_y1[0];
	wall->nyb = (x - wall->x1) * (wall->neighbor_y2[1] - wall->neighbor_y1[1]) /
								(wall->x2 - wall->x1) + wall->neighbor_y1[1];
	wall->ncya = clampf(wall->nya, r->top_limit[x], r->bottom_limit[x]);
	wall->ncyb = clampf(wall->nyb, r->top_limit[x], r->bottom_limit[x]);
	draw_line2(m, &(t_vline){ x, wall->cya, wall->ncya - 1, 0, 0, 0, 5}, (t_interp)INIT_INTERP(wall->ya, wall->cya, wall->yb, 0,1023), wall->txtx);
	r->top_limit[x] = clampf(maxf(wall->cya, wall->ncya),
												r->top_limit[x], m->sdl.img.h - 1);
	draw_line2(m, &(t_vline){ x, wall->ncyb, wall->cyb, 0, 0, 0, 5}, (t_interp)INIT_INTERP(wall->ya, wall->ncyb, wall->yb, 0,1023), wall->txtx);
	r->bottom_limit[x] = clampf(minf(wall->cyb, wall->ncyb),
														0, r->bottom_limit[x]);
}

static void reverse_perspective()
{

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
		wall->txtx = (wall->u0 * ((wall->x2 - x) * renderer->t2.z) + wall->u1*((x-wall->x1) * renderer->t1.z)) / ((wall->x2-x) * renderer->t2.z + (x - wall->x1) * renderer->t1.z);
		#define CeilingFloorScreenCoordinatesToMapCoordinates(player, mapY,screenX,screenY,X,Z) \
				do { \
					Z = mapY * VFOV * H / ((H / 2 - screenY) - player.pitch * VFOV * H); \
					X = Z * (W / 2 - screenX) / (HFOV * W); \
					RelativeMapCoordinatesToAbsoluteOnes(player,X,Z); \
				} while(0)

		#define RelativeMapCoordinatesToAbsoluteOnes(player,X,Z) \
				do { \
					float rtx = Z * player.anglecos + X * player.anglesin; \
					float rtz = Z * player.anglesin - X * player.anglecos; \
					X = rtx + player.position.x; \
					Z = rtz + player.position.y; \
				} while(0)

				for(int y = renderer->top_limit[x]; y <= renderer->bottom_limit[x]; ++y)
				{
					if(y >= wall->cya && y <= wall->cyb)
					{
						y = wall->cyb;
						continue;
					}
					float hei = y < wall->cya ? wall->ceil : wall->floor;
					float mapx, mapz;
					CeilingFloorScreenCoordinatesToMapCoordinates(m->map.player, hei, x,y, mapx, mapz);
					unsigned txtx = (mapx * 256), txtz = (mapz * 256);
					int *pix = y < wall->cya ? m->tex.t.textures[1]->pixels : m->tex.t.textures[0]->pixels;
					sdl_pixel_put(&m->sdl.img, x, y, pix[txtx % m->tex.t.textures[3]->w + (txtz % m->tex.t.textures[3]->h) * m->tex.t.textures[3]->w]);
					// ((int*)surface->pixels)[y*W2+x] = pel;
				}
		
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
