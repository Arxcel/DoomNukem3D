/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sounds.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 14:38:16 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 14:38:47 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void			init_sounds(t_main *m)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1500);
	load_sounds(m);
	Mix_PlayMusic(m->music.msc[0], -1);
}

void			load_sounds(t_main *m)
{
	m->music.msc[0] = Mix_LoadMUS("assets/sounds/m.mp3");
	m->music.msc[1] = Mix_LoadMUS("assets/sounds/lvl2.mp3");
	m->music.msc[2] = Mix_LoadMUS("assets/sounds/lvl3.mp3");
	m->music.msc[3] = Mix_LoadMUS("assets/sounds/game_over.wav");
	m->music.msc[4] = Mix_LoadMUS("assets/sounds/win.wav");
	m->music.msc[5] = Mix_LoadMUS("assets/sounds/menu.mp3");
	m->music.snd[0] = Mix_LoadWAV("assets/sounds/Knife.wav");
	m->music.snd[1] = Mix_LoadWAV("assets/sounds/Pistol.wav");
	m->music.snd[2] = Mix_LoadWAV("assets/sounds/Machine Gun.wav");
	m->music.snd[3] = Mix_LoadWAV("assets/sounds/Gatling Gun.wav");
	m->music.snd[4] = Mix_LoadWAV("assets/sounds/Pickup.wav");
	m->music.snd[5] = Mix_LoadWAV("assets/sounds/Ammo.wav");
	m->music.snd[6] = Mix_LoadWAV("assets/sounds/Key.wav");
	m->music.snd[7] = Mix_LoadWAV("assets/sounds/Boss Gun.wav");
	m->music.snd[8] = Mix_LoadWAV("assets/sounds/Boss Gun.wav");
}
