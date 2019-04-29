/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:34 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 14:43:25 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void					init_sectors(t_map_editor *e)
{
	int i;
	int j;

	i = -1;
	while (++i < SECTORS_CNT && (j = -1))
		while (++j < WALLS_CNT)
		{
			e->sectors[i].neighbors[j] = -1;
			e->sectors[i].num_walls = 1;
			e->sectors[i].wall_vertice[j].texture = 0;
			e->sectors[i].wall_vertice[j].color = YELLOW;
            e->sectors[i].wall_vertice[j].global_index = 0;
			e->sectors[i].ceiling_height = 100;
			e->sectors[i].floor_height = 0;
			e->sectors[i].is_lift = false;
			e->sectors[i].from = 0;
			e->sectors[i].to = 0;
		}
	e->sectors[0].num_walls = -1;
	e->global_index = 0;
	e->n = 0;
	e->mode = TEXTURE;
	e->chosen = -1;
	e->selected_row = 2;
	e->sprite_cnt = 0;
	init_sprites(e);
}

void			init_sprites(t_map_editor *e)
{
	int i;

	i = -1;
	while(++i < SPRITE_CNT)
	{
		e->sprites[i].position.z = 0.0f;
		e->sprites[i].texture = 0;
	}
}

int		init_map_editor(t_main *main)
{
	if (!main)
		return (1);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_SetRelativeMouseMode(SDL_FALSE);
	SDL_SetRenderDrawColor(main->sdl.ren, 0, 0, 0, 0);
	SDL_RenderClear(main->sdl.ren);
	SDL_RenderPresent(main->sdl.ren);
	main->map.player.darkness = 2;
	main->map.player.gravity = 1.0f;
	return (0);
}


int				editor_clear_sdl(t_sdl *sdl)
{
	puts("free");
	SDL_DestroyTexture(sdl->texture);
	SDL_DestroyRenderer(sdl->ren);
	SDL_DestroyWindow(sdl->win);
	free(sdl->img.pixels);
	SDL_Quit();
	return (0);
}

