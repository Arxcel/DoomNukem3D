/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 11:47:04 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "map_parser.h"

static void calcDeltaTime(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) / (float)(SDL_GetPerformanceFrequency());
}

static void testWalls(t_main *m)
{
	int w = m->sdl.img.w / 2;
	int h = m->sdl.img.h / 2;
	int *pix = m->tex.t.textures[2]->pixels;

	int y = -1;
	while (++y < h)
	{
		int x = m->sdl.img.w / 2 - 1;
		while (++x < w + m->sdl.img.w / 2)
		{
			sdl_pixel_put(&m->sdl.img, x, y, pix[x % 1024 + (y % 1024) * m->tex.t.textures[2]->w]);
		}
	}
}

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		calcDeltaTime(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			draw_minimap(m);
			sdl_put_image(&m->sdl);
			move_player(m);
			// printf("Delta time: %f\n", m->delta_time);
			m->delta_time = 0.f;
		}
	}
}

static void	load_block_textures(const char *path, SDL_Surface **target)
{
	if ((*target = sdl_load_surface(path, IS_FORMAT_SURF, PIXEL_FORMAT)) == NULL)
	{
		ft_putendl(SDL_GetError());
		exit(-3);
	}
}

static void			load_textures(t_main *m)
{
	m->tex.t.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10);
	load_block_textures("assets/weapon/PC Computer - Doom Doom II - Weapons Prototype.png", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/2.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/3.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/4.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/5.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/6.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/7.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/8.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/9.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
	load_block_textures("assets/walls/10.JPG", &m->tex.t.textures[m->tex.t.numTextures++]);
}

static void			clear_textures(t_main *m)
{
	while (m->tex.t.numTextures)
		SDL_FreeSurface(m->tex.t.textures[--m->tex.t.numTextures]);
	free(m->tex.t.textures);
}

int					main(int ac, char **av)
{
	t_main			m;

	(void)av;
	ft_bzero(&m, sizeof(t_main));
	load_textures(&m);
    parser(&m.map, av[1]);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	sdl_loop(&m);
	clear_textures(&m);
    remove_data(&m.map);
	SDL_Quit();
	return (0);
}
