/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 10:55:17 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 17:32:08 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			load_hud_texture(SDL_Surface **target, const char *path)
{
	char				*file_contents;
	zip_file_t			*f;
	zip_t				*z;
	struct zip_stat		st;
	SDL_RWops			*rwops;

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_stat_init(&st);
	zip_stat(z, path, 0, &st);
	if (st.size < 1)
		MSG("No such texture");
	file_contents = (char*)malloc(sizeof(char) * st.size);
	f = zip_fopen_encrypted(z, path, 0, RESOURCES_PASS);
	if (!f)
		MSG(zip_strerror(z));
	if (zip_fread(f, file_contents, st.size) < 1 || zip_fclose(f))
		MSG(zip_strerror(z));
	rwops = SDL_RWFromMem(file_contents, st.size);
	if (!(*target =
			sdl_load_surface_from_res(rwops, IS_FORMAT_SURF, PIXEL_FORMAT)))
		MSG(SDL_GetError());
	zip_close(z);
	free(file_contents);
}

static void			load_hud_font(t_main *m, const char *path, size_t size)
{
	zip_file_t			*f;
	zip_t				*z;
	struct zip_stat		st;

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_stat_init(&st);
	zip_stat(z, path, 0, &st);
	if (st.size < 1)
		MSG("No such texture");
	m->hud.font_source = (char*)malloc(sizeof(char) * st.size);
	f = zip_fopen_encrypted(z, path, 0, RESOURCES_PASS);
	if (!f)
		MSG(zip_strerror(z));
	if (zip_fread(f, m->hud.font_source, st.size) < 1 || zip_fclose(f))
		MSG(zip_strerror(z));
	m->hud.font_rwops = SDL_RWFromMem(m->hud.font_source, st.size);
	if (!(m->hud.font = TTF_OpenFontRW(m->hud.font_rwops, SDL_FALSE, size)))
		MSG(SDL_GetError());
	zip_close(z);
}

void				load_hud(t_main *m)
{
	m->map.player.stats.active_weapon = 1;
	m->map.player.stats.total_active_weapon = 1;
	if (TTF_Init())
		MSG(TTF_GetError());
	load_hud_texture(&m->hud.surface_hud, "assets/hud/hud.png");
	load_hud_texture(&m->hud.gun_surface1, "assets/hud/pistol.png");
	// load_hud_texture(&m->hud.gun_surface2, "assets/hud/pistol_2.png");
	// load_hud_texture(&m->hud.gun_surface2, "assets/hud/pistol_3.png");
	load_hud_font(m, "assets/fonts/auto_digital.ttf", 42);
}

void				unload_hud(t_main *m)
{
	SDL_RWclose(m->hud.font_rwops);
	free(m->hud.font_source);
	TTF_CloseFont(m->hud.font);
	SDL_FreeSurface(m->hud.surface_hud);
	SDL_FreeSurface(m->hud.gun_surface1);
	// SDL_FreeSurface(m->hud.gun_surface2);
	// SDL_FreeSurface(m->hud.gun_surface3);	
	TTF_Quit();
}
