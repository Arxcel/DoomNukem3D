/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arxcel <arxcel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/21 18:06:44 by olbondar          #+#    #+#             */
/*   Updated: 2019/05/14 00:19:19 by arxcel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	shoot(t_main *m)
{
	if (m->hud.boom && m->hud.curr_sprite.x <= (int)(m->hud.all_sprites.w))
	{
		m->hud.curr_sprite.x += (m->hud.all_sprites.w) * 2 / 4;
		m->hud.curr_sprite.y = 0;
	}
	else
	{
		m->hud.boom = 0;
		m->hud.curr_sprite.x = 0;
		m->hud.curr_sprite.y = 0;
	}
}

int		init_gun_surface(t_main *m)
{
	if (m->map.player.stats.active_weapon == 1)
	{
		if (!(m->hud.gun_sprite = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.gun_surface1)))
			MSG(SDL_GetError());
	}
	if (m->map.player.stats.active_weapon == 2)
	{
		if (!(m->hud.gun_sprite = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.gun_surface2)))
			MSG(SDL_GetError());
	}
	if (m->map.player.stats.active_weapon == 3)
	{
		if (!(m->hud.gun_sprite = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.gun_surface3)))
			MSG(SDL_GetError());
	}
	return (1);
}

void	draw_gun(t_main *m)
{
	int w;
	int h;

	if (init_gun_surface(m) != 1)
		MSG("Unable to load gun surface");
	SDL_QueryTexture(m->hud.gun_sprite, NULL, NULL, &w, &h);
	if (m->hud.boom && m->map.player.stats.ammo > 0 &&
				m->hud.curr_sprite.x <= (int)(m->hud.all_sprites.w))
		m->hud.curr_sprite.x += (m->hud.all_sprites.w) * 2 / 4;
	else
		m->hud.curr_sprite.x = 0;
	m->hud.curr_sprite.y = 0;
	m->hud.curr_sprite.w = (int)(w / 4);
	m->hud.curr_sprite.h = (int)(h * 2);
	m->hud.all_sprites.x = (int)(m->sdl.img.w / 2.2);
	m->hud.all_sprites.y = (int)(m->sdl.img.h - 0 - h * 2 + 15);
	m->hud.all_sprites.w = (int)(w * 2 / 4);
	m->hud.all_sprites.h = (int)(h * 2);
	m->hud.boom = 0;
	SDL_RenderCopy(m->sdl.ren, m->hud.gun_sprite,
				&m->hud.curr_sprite, &m->hud.all_sprites);
	SDL_DestroyTexture(m->hud.gun_sprite);
}

void	draw_hud(t_main *m)
{
	if (!(m->hud.hud = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.surface_hud)))
		MSG(SDL_GetError());
	m->hud.hud_rect.x = 0;
	m->hud.hud_rect.y = 0;
	m->hud.hud_rect.w = m->sdl.img.w;
	m->hud.hud_rect.h = m->sdl.img.h;
	SDL_RenderCopy(m->sdl.ren, m->hud.hud, NULL, &m->hud.hud_rect);
	SDL_DestroyTexture(m->hud.hud);
	draw_text(m, ft_strdup("Find yellow card"), 440, 550);
	draw_text(m, ft_itoa(m->map.player.stats.ammo), 885, 658);
	draw_text(m, ft_itoa(m->map.player.stats.armor), 220, 705);
	draw_text(m, ft_itoa(m->map.player.stats.hp), 220, 648);
	draw_text(m, ft_itoa(m->map.player.stats.active_weapon), 890, 705);
	draw_text(m, ft_itoa(m->map.player.stats.total_active_weapon), 940, 705);
	if (m->map.player.is_standing &&
				m->map.sectors[m->map.player.sector_number].is_lift &&
				!m->map.sectors[m->map.player.sector_number].is_activated)
		draw_text(m, ft_strdup("Press E to activate the lift"),
							m->sdl.img.w / 2 - 250, m->sdl.img.h / 2 - 30);
	SDL_DestroyTexture(m->hud.hud);
}

void	draw_text(t_main *m, char *text, int x, int y)
{
	SDL_Color color;

	color.r = 213;
	color.g = 255;
	color.b = 0;
	if (!(m->hud.surface_message = TTF_RenderText_Solid(m->hud.font,
			text, color)))
		MSG(TTF_GetError());
	if (!(m->hud.message = SDL_CreateTextureFromSurface(m->sdl.ren,
			m->hud.surface_message)))
		MSG(SDL_GetError());
	m->hud.message_rect.x = x;
	m->hud.message_rect.y = y;
	m->hud.message_rect.w = m->hud.surface_message->w;
	m->hud.message_rect.h = m->hud.surface_message->h;
	SDL_RenderCopy(m->sdl.ren, m->hud.message, NULL, &m->hud.message_rect);
	SDL_DestroyTexture(m->hud.message);
	SDL_FreeSurface(m->hud.surface_message);
	free(text);
}
