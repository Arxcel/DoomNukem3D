/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:24:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:28:46 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "graphics_renderer.h"
#include <math.h>

void		drawMinimap(t_img *img, t_map *map)
{
	int w = img->w / 2;
	int h = img->h / 2;

	int y = -1;
	while (++y < h)
	{
		int x = -1;
		while (++x < w)
		{
			sdl_pixel_append(img, x, y, 0x111111);
		}
	}
	int s = -1;
	while (++s < map->number_sectors)
	{
		int a = -1;
		while (++a < map->sectors[s].number_vertices)
		{
				t_vector res1;
				t_vector res2;

			res1.x = (map->sectors[s].vertices[a].x * 20 - map->player.position.x * 20) * map->player.anglesin -
								(map->sectors[s].vertices[a].y * 40 - map->player.position.y * 40) * map->player.anglecos;
			res1.z = (map->sectors[s].vertices[a].x * 20 - map->player.position.x * 20) * map->player.anglecos +
								(map->sectors[s].vertices[a].y * 40 - map->player.position.y * 40) * map->player.anglesin;

			res2.x = (map->sectors[s].vertices[a+1].x * 20 - map->player.position.x * 20) * map->player.anglesin -
								(map->sectors[s].vertices[a+1].y * 40 - map->player.position.y * 40) * map->player.anglecos;
			res2.z = (map->sectors[s].vertices[a+1].x * 20 - map->player.position.x * 20) * map->player.anglecos +
								(map->sectors[s].vertices[a+1].y * 40 - map->player.position.y * 40) * map->player.anglesin;
			float x1 = w / 2 - res1.x;
			float y1 = h / 2 - res1.z;

			float x2 = w / 2 - res2.x;
			float y2 = h / 2 - res2.z;
			t_line l;
			l.from  = (t_vertex){x1, y1};
			l.to  = (t_vertex){x2, y2};
			l.color = 0xFFFF00;
			l.limit_min = (t_vertex){0, 0};
			l.limit_max = (t_vertex){w, h};
			plot_line(img, &l);
		}
	} 
	int yp = h / 2 - 3;
	while (++yp < h / 2 + 2)
	{
		int xp = w / 2 - 3;
		while (++xp < w / 2 + 2)
		{
			sdl_pixel_put(img, xp, yp, 0xFF0000);
		}
	}
}