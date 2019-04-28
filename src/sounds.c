/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:38:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 16:56:13 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		load_sound_mav(char *path, t_music *m)
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
		MSG("No such sound");
	file_contents = malloc(st.size);
	f = zip_fopen_encrypted(z, path, 0, RESOURCES_PASS);
	if (!f)
		MSG(zip_strerror(z));
	if (zip_fread(f, file_contents, st.size) < 1 || zip_fclose(f))
		MSG(zip_strerror(z));
	rwops = SDL_RWFromMem(file_contents, st.size);
	if (!(m->snd[m->num_sounds++] =
			Mix_LoadWAV_RW(rwops, SDL_TRUE)))
		MSG(SDL_GetError());
	zip_close(z);
	free(file_contents);
}

void			load_sounds(t_main *m)
{
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT,
													2, 1500) == -1)
		MSG(Mix_GetError());
	m->music.snd = (Mix_Chunk**)malloc(sizeof(Mix_Chunk*) * 11);
	load_sound_mav("assets/sounds/Ammo.wav", &m->music);
	load_sound_mav("assets/sounds/Flamethrower.wav", &m->music);
	load_sound_mav("assets/sounds/Health.wav", &m->music);
	load_sound_mav("assets/sounds/Jump.wav", &m->music);
	load_sound_mav("assets/sounds/Lift.wav", &m->music);
	load_sound_mav("assets/sounds/Menu Select.wav", &m->music);
	load_sound_mav("assets/sounds/Menu Toggle.wav", &m->music);
	load_sound_mav("assets/sounds/Pickup.wav", &m->music);
	load_sound_mav("assets/sounds/Pistol.wav", &m->music);
	load_sound_mav("assets/sounds/Start.wav", &m->music);
	load_sound_mav("assets/sounds/Win.wav", &m->music);


}

void			unload_sounds(t_main *m)
{
	while (m->music.num_sounds)
		Mix_FreeChunk(m->music.snd[--m->music.num_sounds]);
	Mix_CloseAudio();
	free(m->music.snd);
}
