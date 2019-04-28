/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:49:14 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/28 15:24:12 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		close_sector(t_main *m, t_map_editor *e)
{
	if (e->sectors[e->n].num_walls > 1 && e->sectors[e->n].num_walls < WALLS_CNT)
	{
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].color = YELLOW;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin.x =
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end.x;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin.y =
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end.y;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end.x =
			e->sectors[e->n].wall_vertice[0].begin.x;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end.y =
			e->sectors[e->n].wall_vertice[0].begin.y;
		e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].global_index = e->global_index++;
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
			e->sectors[e->n].wall_vertice[0].begin = mouse;
		else
		{
			if (e->sectors[e->n].num_walls > 0)
				e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].begin =
					e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls - 1].end;
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].end = mouse;
		}
		if ((e->n == 0 && e->sectors[e->n].num_walls >= 0) || (e->n && e->sectors[e->n].num_walls > 1))
		{
			e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].global_index = e->global_index++;
			printf("global index %i\n", e->sectors[e->n].wall_vertice[e->sectors[e->n].num_walls].global_index);
		}
		e->sectors[e->n].num_walls++;
	}
	else if (e->mode == PORTAL && e->n < SECTORS_CNT - 1)
		e->chosen = check_intersection(e, mouse);
	return (e->chosen);
}

void				sdl_keydown(t_main *m, t_map_editor	*e)
{
	if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (arrow_keys(m->sdl.e.key.keysym.sym, e)
			&& player_save_keys(m, e) &&
			(e->n < SECTORS_CNT && m->sdl.e.key.keysym.sym == SDLK_RETURN))
		{
			if ((e->sectors[e->n].num_walls > 0 && e->mode == TEXTURE
				&& close_sector(m, e))
				|| (e->mode > TEXTURE && e->mode < PORTAL))
					(e->mode)++;
			if (e->mode > CLOSE && e->selected_row < TEXT_MENU_ROW)
				(e->selected_row)++;
			if (e->mode == PORTAL && e->chosen != -1)
			{
				if (e->chosen != e->sectors[e->n].num_walls - 1)
					e->sectors[e->n].neighbors[e->chosen + 1] = e->n + 1;
				else
					e->sectors[e->n].neighbors[0] = e->n + 1;
				(e->n)++;
				e->sectors[e->n].wall_vertice[0] = e->sectors[e->n - 1].wall_vertice[e->chosen];
				e->sectors[e->n].wall_vertice[0].begin = e->sectors[e->n - 1].wall_vertice[e->chosen].end;
				e->sectors[e->n].wall_vertice[0].end = e->sectors[e->n - 1].wall_vertice[e->chosen].begin;
				e->sectors[e->n].wall_vertice[1].global_index =
					e->sectors[e->n - 1].wall_vertice[e->chosen].global_index;
				if (e->sectors[e->n - 1].wall_vertice[e->chosen].global_index != e->sectors[e->n - 1].num_walls - 1)
					e->sectors[e->n].wall_vertice[0].global_index =
						e->sectors[e->n - 1].wall_vertice[e->chosen].global_index + 1;
				else
					e->sectors[e->n].wall_vertice[0].global_index = 0;
				e->sectors[e->n].neighbors[1] = e->n - 1;
				e->chosen = -1;
				e->mode = TEXTURE;
				e->sectors[e->n].num_walls = 1;
				e->selected_row = 3;
			}
		}
	}
}

int					map_editor_loop(t_main *m)
{
	t_map_editor	e;
	init_sectors(&e);
	create_text_menu(m, e.menu);
	m->sdl.running = true;
	while(m->sdl.running)
	{
		while (SDL_PollEvent(&m->sdl.e))
		{
			if (m->sdl.e.type == SDL_QUIT ||  m->sdl.e.key.keysym.sym == SDLK_ESCAPE)
			{
				printf("loop legal finish\n");
				m->sdl.running = false;
			}
			if (e.n < SECTORS_CNT && SDL_MOUSEBUTTONDOWN == m->sdl.e.type)
				e.chosen = create_wall(&e);
			sdl_keydown(m, &e);
			if (m->sdl.e.type == SDL_KEYDOWN || m->sdl.e.type == SDL_MOUSEBUTTONDOWN)
			{
				update_all_menu(m, &e);
				printf("selected row %i\n", e.selected_row);				
			}

			e.chosen = draw(m, &e);
			if (e.mode == PLAYER)
				draw_circle(RED, m);
		}
	}

	return (remove_text_menu(e.menu));
}
