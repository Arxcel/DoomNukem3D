/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 15:55:51 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 17:06:30 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			menu_text(t_main *m, const char *text,
													int y, unsigned int c)
{
	SDL_Color color;

	color.r = ((c & 0x00ff0000) >> 16);
	color.g = ((c & 0x0000ff00) >> 8);
	color.b = ((c & 0x000000ff));
	if (!(m->hud.surface_message = TTF_RenderText_Solid(m->hud.font,
			text, color)))
		MSG(TTF_GetError());
	if (!(m->hud.message = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.surface_message)))
		MSG(SDL_GetError());
	m->hud.message_rect.x = m->sdl.img.w / 2 - 100;
	m->hud.message_rect.y = y;
	m->hud.message_rect.w = m->hud.surface_message->w;
	m->hud.message_rect.h = m->hud.surface_message->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.message, NULL, &m->hud.message_rect);
	SDL_DestroyTexture(m->hud.message);
	SDL_FreeSurface(m->hud.surface_message);
}

static void			menu_general(t_main *m)
{
	int				i;
	int				curr;
	unsigned int	color_a;
	unsigned int	color_b;

	color_a = c_rgb(255, 255, 255);
	color_b = c_rgb(180, 50, 50);
	curr = -1;
	i = m->menu.active_option;
	menu_text(m, "Start", 200, i == ++curr ? color_a : color_b);
	menu_text(m, "Level editor", 300, i == ++curr ? color_a : color_b);
	menu_text(m, "Exit", 400, i == ++curr ? color_a : color_b);
}

static void			menu_level_select(t_main *m)
{
	int				i;
	int				curr;
	unsigned int	color_a;
	unsigned int	color_b;

	color_a = c_rgb(255, 255, 255);
	color_b = c_rgb(180, 50, 50);
	curr = -1;
	i = m->menu.active_option;
	menu_text(m, "Map 1", 200, i == ++curr ? color_a : color_b);
	menu_text(m, "Map 2", 300, i == ++curr ? color_a : color_b);
	menu_text(m, "Map 3", 400, i == ++curr ? color_a : color_b);
	menu_text(m, "Map custom", 500, i == ++curr ? color_a : color_b);
	menu_text(m, "Back", 600, i == ++curr ? color_a : color_b);
}

void				handle_menu(t_main *m, int key)
{
	if (m->sdl.e.type == SDL_KEYUP && key == SDLK_UP)
	{
		m->menu.active_option--;
		Mix_PlayChannel(-1, m->music.snd[6], 0);
	}
	else if (m->sdl.e.type == SDL_KEYUP && key == SDLK_DOWN)
	{
		Mix_PlayChannel(-1, m->music.snd[6], 0);
		m->menu.active_option++;
	}
	if (m->menu.is_level_select)
	{
		if (m->menu.active_option > 4)
			m->menu.active_option = 0;
		else if (m->menu.active_option < 0)
			m->menu.active_option = 4;
	}
	else
	{
		if (m->menu.active_option > 2)
			m->menu.active_option = 0;
		else if (m->menu.active_option < 0)
			m->menu.active_option = 2;
	}
	if (m->sdl.e.type == SDL_KEYUP && key == SDLK_RETURN)
	{
		Mix_PlayChannel(-1, m->music.snd[5], 0);
		if (m->menu.is_level_select)
			do_select_opt(m);
		else
			do_general_opt(m);
	}
}

void				draw_menu(t_main *m)
{
	if (!(m->menu.tex_menu = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->tex.t.textures[1])))
		MSG(SDL_GetError());
	m->menu.menu_rect.x = 0;
	m->menu.menu_rect.y = 0;
	m->menu.menu_rect.w = m->sdl.img.w;
	m->menu.menu_rect.h = m->sdl.img.h;
	SDL_RenderCopy(m->sdl.ren, m->menu.tex_menu, NULL, &m->menu.menu_rect);
	if (m->menu.is_level_select)
		menu_level_select(m);
	else
		menu_general(m);
	SDL_RenderPresent(m->sdl.ren);
	SDL_DestroyTexture(m->menu.tex_menu);
}
