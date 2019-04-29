/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_draw.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:08 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 17:26:50 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		draw_circle(int color, t_main *m, t_vector position)
{
	sdl_pixel_put(&m->sdl.img, position.x,
		position.y, color);
	sdl_pixel_put(&m->sdl.img, position.x + 1,
		position.y, color);
	sdl_pixel_put(&m->sdl.img, position.x - 1,
		position.y, color);
	sdl_pixel_put(&m->sdl.img, position.x,
		position.y, color);
	sdl_pixel_put(&m->sdl.img, position.x,
		position.y + 1, color);
	sdl_pixel_put(&m->sdl.img, position.x,
		position.y - 1, color);
	return (0);
}

int		draw(t_main *m, t_map_editor *e)
{
	int i;
	int j;

	i = -1;
	while ((j = -1) && ++i < e->n + 1)
		while (++j < e->sectors[i].num_walls)
			line(e->sectors[i].wall_vertice[j], m);
	SDL_UpdateTexture(m->sdl.texture, NULL,
	m->sdl.img.pixels, m->sdl.img.w * sizeof(unsigned int));
	sdl_clear_image(&m->sdl.img);
	SDL_RenderCopy(m->sdl.ren, m->sdl.texture, NULL, NULL);
	i = -1;
	if (e->mode < SPRITE_Z)
		while (++i < TEXT_MENU)
			SDL_RenderCopy(m->sdl.ren, e->menu[i].text_texture, NULL, &e->menu[i].dstrect);
	else
		while (++i < SPRITE_MENU)
			SDL_RenderCopy(m->sdl.ren, e->sprite_menu[i].text_texture,
				NULL, &e->sprite_menu[i].dstrect);
	SDL_RenderPresent(m->sdl.ren);
	return (e->chosen);
}

void	print_vector(t_editor_wall wall, t_main *main)
{
	if (wall.begin.x > -1 && wall.begin.y > -1 &&
		wall.begin.x < main->sdl.win_w && wall.begin.y <  main->sdl.win_h)
		sdl_pixel_put(&main->sdl.img, wall.begin.x, wall.begin.y, wall.color);
}