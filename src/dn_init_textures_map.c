/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/22 11:34:28 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	add_show_saved_pos(int max, point *spos, point *epos)
{
	int	i;

	i = -1;
	while(++i < max)
		printf(" [%d] %d %d | %d %d\n", i + 1, spos[i].y, spos[i].x,
					epos[i].y, epos[i].x);
}

int		add_check_saved(point p,
		int already_saved_textures, point *spos, point *epos)
{
	int	i;
	
	i = -1;
	while (++i < already_saved_textures)
		if ((p.y >= spos[i].y && p.y < epos[i].y)
		&& (p.x >= spos[i].x && p.x < epos[i].x))
			return (i + 1);
	return (false);
}

static bool	add_weapons_map_init(t_textures *t)
{
	point	p;
	point	spos[WPNS_MAX_TEXTURES];
	point	epos[WPNS_MAX_TEXTURES];
	int		which_texture_skip;

	p.y = -1;
	t->wpns.tmax = 0;
	which_texture_skip = 0;
	_NOTIS_F(t->wpns.surf = sdl_load_surface(WPNS_MAP));
	_NOTIS_F(t->wpns.pxls = t->wpns.surf->pixels);
	t->wpns.s = (point) {t->wpns.surf->w, t->wpns.surf->h};
	while(++(p.y) < t->wpns.s.h && (p.x = -1))
		while (++(p.x) < t->wpns.s.w)
			if (t->wpns.pxls[p.y * t->wpns.s.w + p.x] == WPNS_TEX_BG)
				if ((which_texture_skip =
					add_check_saved(p, t->wpns.tmax, spos, epos)))
					p.x = epos[which_texture_skip - 1].x;
				else
				{
					spos[t->wpns.tmax] = (point){p.x, p.y};
					epos[t->wpns.tmax] = (point){p.x, p.y};
					while (epos[t->wpns.tmax].x < t->wpns.s.w
						&& t->wpns.pxls[epos[t->wpns.tmax].y *
							t->wpns.s.w + epos[t->wpns.tmax].x] != WPNS_MAP_BG)
						++(epos[t->wpns.tmax].x);
					p.x = epos[t->wpns.tmax].x;
					while (epos[t->wpns.tmax].y < t->wpns.s.h
						&& t->wpns.pxls[epos[t->wpns.tmax].y *
							t->wpns.s.w + (p.x - 1)]
							!= WPNS_MAP_BG)
						++(epos[t->wpns.tmax].y);
					++(t->wpns.tmax);
				}
	_ISZ(point, t->wpns.spos, t->wpns.tmax);
	_ISZ(point, t->wpns.epos, t->wpns.tmax);
	int	i = -1;
	while (++i < t->wpns.tmax)
	{
		t->wpns.spos[i] = spos[i];
		t->wpns.epos[i] = epos[i];
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
