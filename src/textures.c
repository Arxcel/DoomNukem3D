/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 13:36:31 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 15:26:36 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		load_block_textures(const char *path, t_tblocks *textures)
{
	SDL_Surface **target;

	target = &textures->textures[textures->num_textures++];
	if (!(*target = sdl_load_surface(path, IS_FORMAT_SURF, PIXEL_FORMAT)))
	{
		ft_putendl(SDL_GetError());
		exit(-3);
	}
}

void			load_textures_snd(t_main *m)
{
	ft_bzero(&m->tex, sizeof(m->tex));
	m->tex.t.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 14);
	load_block_textures("assets/walls/w1.png", &m->tex.t);
	load_block_textures("assets/walls/w4.png", &m->tex.t);
	load_block_textures("assets/walls/w5.jpg", &m->tex.t);
	load_block_textures("assets/walls/w10.png", &m->tex.t);
	load_block_textures("assets/walls/w11.png", &m->tex.t);
	load_block_textures("assets/floors/f1.png", &m->tex.t);
	load_block_textures("assets/floors/f2.jpg", &m->tex.t);
	load_block_textures("assets/floors/f3.png", &m->tex.t);
	load_block_textures("assets/floors/f4.png", &m->tex.t);
	load_block_textures("assets/floors/f6.png", &m->tex.t);
	load_block_textures("assets/skies/s2.png", &m->tex.t);
	load_block_textures("assets/skies/s3.png", &m->tex.t);
	load_block_textures("assets/skies/s4.png", &m->tex.t);
	load_block_textures("assets/skies/s5.png", &m->tex.t);
	m->tex.s.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 1);
	load_block_textures("assets/sprites/barrel.png", &m->tex.s);
	init_sounds(m);
}

void			clear_textures(t_main *m)
{
	while (m->tex.t.num_textures)
		SDL_FreeSurface(m->tex.t.textures[--m->tex.t.num_textures]);
	free(m->tex.t.textures);
}

void			setup_wall_texture(t_main *m, t_wall *w, int wall, t_pt verical)
{
	w->u0 = 0;
	w->u1 = 1023;
	w->floor_id = (5 + verical.x) % m->tex.t.num_textures;
	w->ceil_id = (5 + verical.y) % m->tex.t.num_textures;
	w->upper_id = wall % m->tex.t.num_textures;
	w->lower_id = wall % m->tex.t.num_textures;
	w->solid_id = wall % m->tex.t.num_textures;
}
