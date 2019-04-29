/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 13:36:31 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 18:34:23 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		load_block_textures(const char *path, t_tblocks *textures)
{
	char				*file_contents;
	zip_file_t			*f;
	zip_t				*z;
	struct zip_stat		st;
	SDL_RWops			*rwops;

	if (!(z = zip_open(RESOURCES, ZIP_CREATE, 0)))
		MSG(zip_strerror(z));
	zip_set_default_password(z, RESOURCES_PASS);
	zip_stat_init(&st);
	zip_stat(z, path, 0, &st);
	if (st.size < 1)
		MSG("No such texture");
	file_contents = malloc(st.size);
	f = zip_fopen(z, path, 0);
	if (!f)
		MSG(zip_strerror(z));
	if (zip_fread(f, file_contents, st.size) < 1 || zip_fclose(f))
		MSG(zip_strerror(z));
	rwops = SDL_RWFromMem(file_contents, st.size);
	if (!(textures->textures[textures->num_textures++] =
			sdl_load_surface_from_res(rwops, IS_FORMAT_SURF, PIXEL_FORMAT)))
		MSG(SDL_GetError());
	zip_close(z);
	free(file_contents);
}

void			load_sprites(t_main *m)
{
	m->tex.s.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 10);
	load_block_textures("assets/sprites/ammo.png", &m->tex.s);
	load_block_textures("assets/sprites/barrel.png", &m->tex.s);
	load_block_textures("assets/sprites/helmet.png", &m->tex.s);
	load_block_textures("assets/sprites/medkit.png", &m->tex.s);
	load_block_textures("assets/sprites/rockets.png", &m->tex.s);
	load_block_textures("assets/sprites/medkit2.png", &m->tex.s);
	load_block_textures("assets/sprites/armor.png", &m->tex.s);
	load_block_textures("assets/sprites/card.png", &m->tex.s);
	load_block_textures("assets/sprites/weapon.png", &m->tex.s);
	load_block_textures("assets/sprites/weapon2.png", &m->tex.s);
}

void			load_textures(t_main *m)
{
	ft_bzero(&m->tex, sizeof(m->tex));
	m->tex.t.textures = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * 13);
	load_block_textures("assets/walls/w0.png", &m->tex.t);
	load_block_textures("assets/walls/w1.png", &m->tex.t);
	load_block_textures("assets/walls/w2.jpg", &m->tex.t);
	load_block_textures("assets/walls/w3.png", &m->tex.t);
	load_block_textures("assets/walls/w4.png", &m->tex.t);
	load_block_textures("assets/walls/w5.png", &m->tex.t);
	load_block_textures("assets/floors/f2.jpg", &m->tex.t);
	load_block_textures("assets/walls/w7.png", &m->tex.t);
	load_block_textures("assets/walls/w8.png", &m->tex.t);
	load_block_textures("assets/walls/w9.png", &m->tex.t);
	load_block_textures("assets/walls/w10.png", &m->tex.t);
	load_block_textures("assets/floors/f1.png", &m->tex.t);
	load_block_textures("assets/floors/f2.jpg", &m->tex.t);
}

void			unload_textures_and_sprites(t_main *m)
{
	while (m->tex.t.num_textures)
		SDL_FreeSurface(m->tex.t.textures[--m->tex.t.num_textures]);
	while (m->tex.s.num_textures)
		SDL_FreeSurface(m->tex.s.textures[--m->tex.s.num_textures]);
	free(m->tex.t.textures);
	free(m->tex.s.textures);
}

void			setup_wall_texture(t_main *m, t_wall *w, int wall, t_pt verical)
{
	w->floor_id = (11 + verical.x) % m->tex.t.num_textures;
	w->ceil_id = (11 + verical.y) % m->tex.t.num_textures;
	w->upper_id = wall % m->tex.t.num_textures;
	w->lower_id = wall % m->tex.t.num_textures;
	w->solid_id = wall % m->tex.t.num_textures;
	w->u0 = 0;
	w->u1 = m->tex.t.textures[w->solid_id]->w;
}
