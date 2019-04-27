/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_keys.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:45 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/27 18:48:47 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_editor.h"

int				left_arrow_key(SDL_Keycode sym, t_map_editor *e)
{
	if (sym != SDLK_LEFT  || (e->n && e->sectors[e->n].num_walls < 2))
		return (1);
	if (!e->n && e->sectors[e->n].num_walls < 0)
		return (1);
	e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture = 0;
	e->sectors[e->n].num_walls--;
    e->global_index--;
	if (e->mode == CLOSE)
		(e->mode)--;
	return (0);
}

int				arrow_keys(SDL_Keycode sym, t_map_editor *e)
{
	if (sym != SDLK_UP && sym != SDLK_DOWN && sym != SDLK_LEFT)
		return (1);
	if ((e->mode == TEXTURE || e->mode == CLOSE) && !left_arrow_key(sym, e))
		return (1);
	if ((e->mode == TEXTURE || e->mode == CLOSE) && e->sectors[e->n].num_walls > 0)
	{
		if (sym == SDLK_UP && e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture < TEXTURE_MAX)
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture++;
		else if (sym == SDLK_DOWN && e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture > 0)
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].texture--;
		else
			return (1);
	}
	else if (e->mode == FLOOR_HEIGHT)
	{
		if (sym == SDLK_UP && e->sectors[e->n].floor_height + 10 < e->sectors[e->n].ceiling_height)
			e->sectors[e->n].floor_height += 10;
		else if (sym == SDLK_DOWN && e->sectors[e->n].floor_height - 10 >= MIN_FLOOR_HEIGHT)
			e->sectors[e->n].floor_height -= 10;
		else
			return (1);		
	}
	else if (e->mode == CEILING_HEIGHT)
	{
		if (sym == SDLK_UP && e->sectors[e->n].ceiling_height + 10 <= MAX_CEILING_HEIGHT)
			e->sectors[e->n].ceiling_height += 10;
		else if (sym == SDLK_DOWN && e->sectors[e->n].ceiling_height - 10 > e->sectors[e->n].floor_height)
			e->sectors[e->n].ceiling_height -= 10;
		else
			return (1);
	}
	else
		return (1);
	return (0);
}

int					player_save_keys(t_main *m, t_map_editor *e)
{
	int i;

	if (m->sdl.e.key.keysym.sym == SDLK_s && e->mode >= CEILING_HEIGHT)
	{
		if (e->mode < PLAYER)
		{
			shift_left(e);
			e->mode = PLAYER;
		}
		else
			serialize_map(m, e->sectors, e->n);
		return (0);
	}
	if (e->mode < PLAYER)
		return (1);
	t_dot d;
	SDL_GetMouseState(&d.x, &d.y);
	i = -1;
	while (++i <= e->n)
		if (pnpoly(e->sectors[i].num_walls, e->sectors[i].wall_vertice, d))
		{
			m->map.player.sector_number = i;
			m->map.player.position.x = d.x;
			m->map.player.position.y = d.y;
			return (0);
		}
	return (1);
}
