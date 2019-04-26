/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olbondar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/21 18:06:44 by olbondar          #+#    #+#             */
/*   Updated: 2019/04/21 18:06:46 by olbondar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	draw_hud(t_main *m)
{
	if (!(m->hud.hud = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.surface_hud)))
		MSG(SDL_GetError());
	m->hud.hud_rect.x = 0;
	m->hud.hud_rect.y = 0;
	m->hud.hud_rect.w = m->hud.surface_hud->w;
	m->hud.hud_rect.h = m->hud.surface_hud->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.hud, NULL, &m->hud.hud_rect);
	SDL_DestroyTexture(m->hud.hud);
}

void	draw_text(t_main *m)
{
	SDL_Color color;

	color.r = 213;
	color.g = 255;
	color.b = 0;
	if (!(m->hud.surface_message = TTF_RenderText_Solid(m->hud.font,
			"lorem ipsum", color)))
		MSG(TTF_GetError());
	if (!(m->hud.message = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.surface_message)))
		MSG(SDL_GetError());
	m->hud.message_rect.x = 535;
	m->hud.message_rect.y = 615;
	m->hud.message_rect.w = m->hud.surface_message->w;
	m->hud.message_rect.h = m->hud.surface_message->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.message, NULL, &m->hud.message_rect);
	SDL_DestroyTexture(m->hud.message);
	SDL_FreeSurface(m->hud.surface_message);
}
