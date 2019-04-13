/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:24:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 15:22:09 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include <math.h>

static void			draw_player(t_img *img, int w, int h)
{
	int		y;
	int		x;
	int		player_size;

	player_size = 3;
	y = h / 2 - player_size;
	while (++y < h / 2 + player_size)
	{
		x = w / 2 - player_size;
		while (++x < w / 2 + player_size)
		{
			sdl_pixel_put(img, x, y, 0xFF00FF);
		}
	}
}

static t_vertex		get_scaled_edge(t_main *m, t_vertex ver, int s_x, int s_y)
{
	t_vertex e;

	e.x = (ver.x - m->map.player.position.x) * m->map.player.anglesin * s_x -
			(ver.y - m->map.player.position.y) * m->map.player.anglecos * s_y;
	e.y = (ver.x - m->map.player.position.x) * m->map.player.anglecos * s_x +
			(ver.y - m->map.player.position.y) * m->map.player.anglesin * s_y;
	return (e);
}

static void			draw_sector(t_main *m, int w, int h, int s_i)
{
	int		a;
	t_line	l;

	l.color = 0xFFFF00;
	l.limit_min = (t_vertex){0, 0};
	l.limit_max = (t_vertex){w, h};
	a = -1;
	while (++a < m->map.sectors[s_i].number_vertices)
	{
		l.from = get_scaled_edge(m, m->map.sectors[s_i].vertices[a], 20, 40);
		l.to = get_scaled_edge(m, m->map.sectors[s_i].vertices[a + 1], 20, 40);
		l.from.x = w / 2 - l.from.x;
		l.from.y = h / 2 - l.from.y;
		l.to.x = w / 2 - l.to.x;
		l.to.y = h / 2 - l.to.y;
		plot_line(&m->sdl.img, &l);
	}
}

void				draw_minimap(t_main *m)
{
	int w;
	int h;
	int s;

	w = m->sdl.img.w / 2;
	h = m->sdl.img.h / 2;
	s = -1;
	while (++s < m->map.number_sectors)
	{
		draw_sector(m, w, h, s);
	}
	draw_player(&m->sdl.img, w, h);
}
