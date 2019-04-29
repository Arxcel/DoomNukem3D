/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:45 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 19:45:32 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int				arrow_keys(t_main *m, t_map_editor *e)
{
	if (m->sdl.e.key.keysym.sym != SDLK_UP && m->sdl.e.key.keysym.sym !=
							SDLK_DOWN && m->sdl.e.key.keysym.sym != SDLK_LEFT)
		return (1);
	if ((e->mode == TEXTURE || e->mode == CLOSE) && !left_arrow_key(m, e))
		return (1);
	if ((m->sdl.e.key.keysym.sym == SDLK_UP && !up_arrow_key(m, e)) ||
			(m->sdl.e.key.keysym.sym == SDLK_DOWN && !down_arrow_key(m, e)))
		return (0);
	else if (e->mode == IS_LIFTED)
	{
		e->sectors[e->n].is_lift = e->sectors[e->n].is_lift ? false : true;
		e->sectors[e->n].from =
			e->sectors[e->n].is_lift ? e->sectors[e->n].floor_height : 0;
	}
	else
		return (1);
	return (0);
}

static int		mouse_handle(t_main *m, t_map_editor *e, t_dot d)
{
	int i;

	i = -1;
	while (e->mode >= CREATED && e->mode < SPRITE_Z && ++i <= e->n)
		if (pnpoly(e->sectors[i].num_walls, e->sectors[i].walls, d))
		{
			m->map.player.sector_number = i;
			m->map.player.position.x = d.x;
			m->map.player.position.y = d.y;
			e->mode = PLAYER;
			return (0);
		}
	if ((e->mode == SPRITE_Z || e->mode == SPRITE_TEXTURE) &&
											e->sprite_cnt < SPRITE_CNT)
	{
		e->sprites[e->sprite_cnt].position.x = d.x;
		e->sprites[e->sprite_cnt].position.y = d.y;
		e->sprite_cnt++;
		e->mode = SPRITE_Z;
		return (0);
	}
	return (1);
}

int				player_save_keys(t_main *m, t_map_editor *e)
{
	t_dot	d;

	if (m->sdl.e.key.keysym.sym != SDLK_s &&
								m->sdl.e.type != SDL_MOUSEBUTTONDOWN)
		return (1);
	if (m->sdl.e.key.keysym.sym == SDLK_s && e->mode >= PORTAL)
	{
		if (e->mode == PORTAL)
		{
			e->mode = CREATED;
			shift_left(e);
		}
		else if (e->mode == SPRITE_TEXTURE)
			e->mode = DARKNESS;
		else if (e->mode == SAVE)
			serialize_map(m, e);
		return (0);
	}
	SDL_GetMouseState(&d.x, &d.y);
	return (mouse_handle(m, e, d));
}
