/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/21 20:13:31 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	add_weapons_map_init(t_textures *t)
{
	t_point	p;
	int		textures_counter;

	p.y = -1;
	textures_counter = 0;
	_NOTIS_F(t->wpns.surf = sdl_load_surface(WEAPONS_MAP));
	_NOTIS_F(t->wpns.pxls = t->wpns.surf->pixels);
	t->wpns.w = t->wpns.surf->w;
	t->wpns.h = t->wpns.surf->h;
	while(++(p.y) < t->wpns.h && (p.x = -1)) {
		while (++(p.x) < t->wpns.w) {
			if (t->wpns.pxls[p.y * t->wpns.w + p.x] == WPNS_TEX_BG) {
				++textures_counter;
				// incompleted pushed to branch just for save it.
			}
		}
	}
}

bool		dn_init_textures_map(t_textures *t)
{
	t_point	p;
	int		i;

	i = -1;
	p = (t_point){0, 0};
	_NOTIS_F(t->walls.surf = sdl_load_surface(TEXUTRES_MAP));
	_NOTIS_F(t->walls.pxls = t->walls.surf->pixels);
	t->walls.w = t->walls.surf->w;
	t->walls.h = t->walls.surf->h;
	t->walls.tmax = WALL_MAX_TEXTURES;
	_ISZ(t_point, t->walls.spos, t->walls.tmax);
	_ISZ(t_point, t->walls.epos, t->walls.tmax);
	while (++i < t->walls.tmax) {
		t->walls.spos[i] = (t_point){p.x, p.y};
		t->walls.epos[i] = (t_point){p.x + WALL_SIZE, p.y + WALL_SIZE};
		p.x += WALL_SIZE;
		if (i + 1 == t->walls.tmax / 2)
			p = (t_point){0, p.y + WALL_SIZE};
	}
	return (true);
}
