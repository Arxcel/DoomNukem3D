/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/21 12:20:29 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "map_parser.h"
///////////////////////////////////////////////////////////////////
void				load_hud(t_main *m)
{
	if (!(TTF_Init()))
		ft_putendl(TTF_GetError());
}

void				draw_hud(t_main *m)
{
	if (!(m->hud.surface_hud = IMG_Load("hud.png")))
		ft_putendl(IMG_GetError());
	if (!(m->hud.hud = SDL_CreateTextureFromSurface(m->sdl.ren, m->hud.surface_hud)))
		ft_putendl(SDL_GetError());
	m->hud.hud_rect.x = 0;
	m->hud.hud_rect.y = 0;
	m->hud.hud_rect.w = m->hud.surface_hud->w;
	m->hud.hud_rect.h = m->hud.surface_hud->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.hud, NULL, &m->hud.hud_rect);
	SDL_DestroyTexture(m->hud.hud);
	SDL_FreeSurface(m->hud.surface_hud);
}

void				draw_text(t_main *m)
{
	SDL_Color		color = {213, 255, 0};

	
	if (!(m->hud.font = TTF_OpenFont("auto_digital.ttf", 45)))
		ft_putendl(TTF_GetError());
	if (!(m->hud.surface_message = TTF_RenderText_Solid(m->hud.font, "lorem ipsum", color)))
		ft_putendl(TTF_GetError());
	if (!(m->hud.message = SDL_CreateTextureFromSurface(m->sdl.ren, m->hud.surface_message)))
		ft_putendl(SDL_GetError());
	m->hud.message_rect.x = 535;
	m->hud.message_rect.y = 615;
	m->hud.message_rect.w = m->hud.surface_message->w;
	m->hud.message_rect.h = m->hud.surface_message->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.message, NULL, &m->hud.message_rect);
	SDL_DestroyTexture(m->hud.message);
	SDL_FreeSurface(m->hud.surface_message);
	TTF_CloseFont(m->hud.font);
}
///////////////////////////////////////////////////////////////////
static void			calc_delta_time(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) /
									(float)(SDL_GetPerformanceFrequency());
}

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		calc_delta_time(m);
		sdl_hook(m);
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			draw_minimap(m);
			sdl_put_image(&m->sdl);
			///////////////////////////////
			draw_hud(m);
			draw_text(m);
			///////////////////////////////
			render_sprites(m);
			SDL_RenderPresent(m->sdl.ren);
			move_player(m);
			calk_sprite_collisions(m);
			m->delta_time = 0.f;
		}
	}
}

static void			init_sprite(t_main *m)
{
	int i;

	i = -1;
	m->map.number_sprites = 5;
	m->map.sprites = (t_sprite*)malloc(sizeof(t_sprite) *
													m->map.number_sprites);
	while (++i < m->map.number_sprites)
	{
		m->map.sprites[i].position = (t_vector){15.0, 15.0 + 1 * i, 15.0};
		m->map.sprites[i].w = 10;
		m->map.sprites[i].h = 5;
		m->map.sprites[i].is_active = true;
		m->map.sprites[i].texture = i;
	}
}

int					main(int ac, char **av)
{
	t_main			m;

	ft_bzero(&m, sizeof(t_main));
	load_textures_snd(&m);
	parser(&m.map, av[1]);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	load_hud(&m);
	init_sprite(&m);
	sdl_loop(&m);
	clear_textures(&m);
	remove_data(&m.map);
	Mix_CloseAudio();
	SDL_Quit();
	return (0);
}
