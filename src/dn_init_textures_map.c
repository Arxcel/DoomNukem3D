/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/21 22:07:15 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static int	add_check_is_in_saved_texture_field(point p,
	int already_saved_textures, point *spos, point *epos)
{
	int	i;
	
	i = -1;
	while (++i < already_saved_textures)
		if (p.y >= spos[i].y && p.y < epos[i].y
		&& p.x >= spos[i].x && p.x < epos[i].y)
			return (i);
	return (0);
}

static bool	add_weapons_map_init(t_textures *t)
{
	point	p;
	point	spos[64];
	point	epos[64];
	int		textures_counter;
	int		which_texture_skip;

	p.y = -1;
	textures_counter = 0;
	which_texture_skip = 0;
	_NOTIS_F(t->wpns.surf = sdl_load_surface(WEAPONS_MAP));
	_NOTIS_F(t->wpns.pxls = t->wpns.surf->pixels);
	t->wpns.s = (point) {t->wpns.surf->w, t->wpns.s.h};
	while(++(p.y) < t->wpns.s.h && (p.x = -1))
		while (++(p.x) < t->wpns.s.w)
			if (t->wpns.pxls[p.y * t->wpns.s.w + p.x] == WPNS_TEX_BG)
			{
				if ((which_texture_skip = add_check_is_in_saved_texture_field(p,
					textures_counter, spos, epos)))
					p = (point) {epos[which_texture_skip].x,
									epos[which_texture_skip].y};
				else
				{
					spos[textures_counter] = (point){p.x, p.y};
					while (p.x < t->wpns.s.w
					&& t->wpns.pxls[p.y * t->wpns.s.w + p.x] != WPNS_MAP_BG)
						++p.x;
					while (p.y < t->wpns.s.h
					&& t->wpns.pxls[p.y * t->wpns.s.w + (p.x - 1)] != WPNS_MAP_BG)
						++p.y;
					epos[textures_counter++] = (point){p.x, p.y};
				}
			}
	return (true);
}

bool		dn_init_textures_map(t_textures *t)
{
	point	p;
	int		i;

	i = -1;
	p = (point){0, 0};
	_NOTIS_F(t->walls.surf = sdl_load_surface(TEXUTRES_MAP));
	_NOTIS_F(t->walls.pxls = t->walls.surf->pixels);
	t->walls.s = (point){t->walls.surf->w, t->walls.surf->h};
	t->walls.tmax = WALL_MAX_TEXTURES;
	_ISZ(point, t->walls.spos, t->walls.tmax);
	_ISZ(point, t->walls.epos, t->walls.tmax);
	while (++i < t->walls.tmax)
	{
		t->walls.spos[i] = (point){p.x, p.y};
		t->walls.epos[i] = (point){p.x + WALL_SIZE, p.y + WALL_SIZE};
		p.x += WALL_SIZE;
		if (i + 1 == t->walls.tmax / 2)
			p = (point){0, p.y + WALL_SIZE};
	}
	_NOTIS_F(add_weapons_map_init(t));
	return (true);
}
