/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   victory.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:02:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 18:15:10 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void			victory_text(t_main *m, const char *text,
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
	m->hud.message_rect.x = m->sdl.img.w / 2 - 60;
	m->hud.message_rect.y = y;
	m->hud.message_rect.w = m->hud.surface_message->w;
	m->hud.message_rect.h = m->hud.surface_message->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.message, NULL, &m->hud.message_rect);
	SDL_DestroyTexture(m->hud.message);
	SDL_FreeSurface(m->hud.surface_message);
}

void				draw_victory(t_main *m)
{
	if (!(m->menu.tex_menu = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->tex.t.textures[3])))
		MSG(SDL_GetError());
	m->menu.menu_rect.x = 0;
	m->menu.menu_rect.y = 0;
	m->menu.menu_rect.w = m->sdl.img.w;
	m->menu.menu_rect.h = m->sdl.img.h;
	SDL_RenderCopy(m->sdl.ren, m->menu.tex_menu, NULL, &m->menu.menu_rect);
	victory_text(m, "You won!", m->sdl.img.h / 4 + 60, c_rgb(255, 100, 100));
	SDL_RenderPresent(m->sdl.ren);
	SDL_DestroyTexture(m->menu.tex_menu);
}
