/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 13:36:31 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 14:38:42 by vkozlov          ###   ########.fr       */
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

void			load_textures(t_main *m)
{
	m->tex.t.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10);
	load_block_textures("assets/walls/1.JPG", &m->tex.t);
	load_block_textures("assets/walls/2.JPG", &m->tex.t);
	load_block_textures("assets/walls/3.JPG", &m->tex.t);
	load_block_textures("assets/walls/4.JPG", &m->tex.t);
	load_block_textures("assets/walls/5.JPG", &m->tex.t);
	load_block_textures("assets/walls/6.JPG", &m->tex.t);
	load_block_textures("assets/walls/7.JPG", &m->tex.t);
	load_block_textures("assets/walls/8.JPG", &m->tex.t);
	load_block_textures("assets/walls/9.JPG", &m->tex.t);
	load_block_textures("assets/walls/10.JPG", &m->tex.t);
}

void			clear_textures(t_main *m)
{
	while (m->tex.t.num_textures)
		SDL_FreeSurface(m->tex.t.textures[--m->tex.t.num_textures]);
	free(m->tex.t.textures);
}

void			setup_wall_texture(t_main *m, t_wall *w, int s)
{
	w->u0 = 0;
	w->u1 = 1023;
	w->floor_id = 0 % m->tex.t.num_textures;
	w->ceil_id = 1 % m->tex.t.num_textures;
	w->upper_id = s % m->tex.t.num_textures;
	w->lower_id = s % m->tex.t.num_textures;
	w->solid_id = s % m->tex.t.num_textures;
}
