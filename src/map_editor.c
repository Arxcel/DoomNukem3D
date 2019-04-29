/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:49:14 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 19:23:31 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		close_sector(t_map_editor *e)
{
	if (e->sectors[e->n].num_walls > 1 &&
		e->sectors[e->n].num_walls < WALLS_CNT)
	{
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].color =
			YELLOW;
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].begin.x =
			e->sectors[e->n].walls[e->sectors[e->n].num_walls - 1].end.x;
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].begin.y =
			e->sectors[e->n].walls[e->sectors[e->n].num_walls - 1].end.y;
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].end.x =
			e->sectors[e->n].walls[0].begin.x;
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].end.y =
			e->sectors[e->n].walls[0].begin.y;
		e->sectors[e->n].walls[e->sectors[e->n].num_walls].global_index =
			e->global_index++;
		e->sectors[e->n].num_walls++;
		return (e->sectors[e->n].num_walls);
	}
	return (0);
}

int		create_wall(t_map_editor *e)
{
	t_dot mouse;

	SDL_GetMouseState(&mouse.x, &mouse.y);
	if (e->mode == TEXTURE && e->sectors[e->n].num_walls < WALLS_CNT - 1)
	{
		if (e->sectors[e->n].num_walls == -1)
			e->sectors[e->n].walls[0].begin = mouse;
		else
		{
			if (e->sectors[e->n].num_walls > 0)
				e->sectors[e->n].walls[e->sectors[e->n].num_walls].
				begin = e->sectors[e->n].walls[e->sectors[e->n].
				num_walls - 1].end;
			e->sectors[e->n].walls[e->sectors[e->n].num_walls].end = mouse;
		}
		if ((e->n == 0 && e->sectors[e->n].num_walls >= 0) ||
			(e->n && e->sectors[e->n].num_walls > 1))
			e->sectors[e->n].walls[e->sectors[e->n].num_walls].global_index =
				e->global_index++;
		e->sectors[e->n].num_walls++;
	}
	else if (e->mode == PORTAL && e->n < SECTORS_CNT - 1)
		e->chosen = check_intersection(e, mouse);
	return (e->chosen);
}

void				create_sector(t_map_editor	*e)
{
	if (e->mode == PORTAL && e->chosen != -1)
	{
		if (e->chosen != e->sectors[e->n].num_walls - 1)
			e->sectors[e->n].neighbors[e->chosen + 1] = e->n + 1;
		else
			e->sectors[e->n].neighbors[0] = e->n + 1;
		(e->n)++;
		e->sectors[e->n].walls[0] = e->sectors[e->n - 1].walls[e->chosen];
		e->sectors[e->n].walls[0].begin = e->sectors[e->n - 1].walls[e->chosen].end;
		e->sectors[e->n].walls[0].end = e->sectors[e->n - 1].walls[e->chosen].begin;
		e->sectors[e->n].walls[1].global_index =
			e->sectors[e->n - 1].walls[e->chosen].global_index;
		if (e->sectors[e->n - 1].walls[e->chosen].global_index != e->sectors[e->n - 1].num_walls - 1)
			e->sectors[e->n].walls[0].global_index =
				e->sectors[e->n - 1].walls[e->chosen].global_index + 1;
		else
			e->sectors[e->n].walls[0].global_index = 0;
		e->sectors[e->n].neighbors[1] = e->n - 1;
		e->chosen = -1;
		e->mode = TEXTURE;
		e->sectors[e->n].num_walls = 1;
		e->selected_row = 2;
	}
}

void				sdl_keydown(t_main *m, t_map_editor	*e)
{
	if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (arrow_keys(m, e)
			&& player_save_keys(m, e) &&
			(e->n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN))
		{
			if ((e->sectors[e->n].num_walls > 0 && e->mode == TEXTURE
				&& close_sector(e))
				|| (e->mode > TEXTURE && e->mode < PORTAL)
				|| e->mode == PLAYER || e->mode == SPRITE_Z
				|| (e->mode > SPRITE_TEXTURE && e->mode < SAVE))
					(e->mode)++;
			if ((e->mode > CLOSE && e->mode < SPRITE_Z && e->selected_row < TEXT_MENU / 2))
				(e->selected_row)++;
			create_sector(e);
		}
	}
}

int					map_editor_loop(t_main *m)
{
	t_map_editor	e;
	int i;

	init_sectors(&e);
	create_text_menu(m, e.menu);
	create_sprite_menu(m, e.sprite_menu);
	m->sdl.running = true;
	ft_bzero(&m->sdl.e, sizeof(m->sdl.e));
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
			{
				m->sdl.running = false;
			}
			if (e.n < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type)
				e.chosen = create_wall(&e);
			sdl_keydown(m, &e);
			if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
			{
				if (e.mode < SPRITE_Z)
					update_all_menu(m, &e);
				else
					update_sprite_menu(m, &e);
				if (m->sdl.e.key.keysym.sym == SDLK_1)
					e.mode = SPRITE_Z;
				printf("mode %i\n", e.mode);
				printf("selected row %i\n", e.selected_row);
			}
			e.chosen = draw(m, &e);
			if (e.mode >= PLAYER)
				draw_circle(RED, m, m->map.player.position);
			i = -1;
			if (e.mode >= SPRITE_Z && e.sprite_cnt)
				while (++i < e.sprite_cnt)
					draw_circle(YELLOW, m, e.sprites[i].position);
		}
	}
	return (remove_text_menu(&e));
}
