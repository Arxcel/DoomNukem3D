/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/22 12:27:45 by tmaluh           ###   ########.fr       */
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

int			dn_check_saved_texture(point p,
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

bool		dn_init_ck_map(t_tmap *t, char *path,
							Uint32 ck_color, Uint32 bg_color)
{
	point	p;
	point	spos[WPNS_MAX_TEXTURES];
	point	epos[WPNS_MAX_TEXTURES];
	int		which_texture_skip;

	p.y = -1;
	t->tmax = 0;
	which_texture_skip = 0;
	_NOTIS_F(t->surf = sdl_load_surface(WPNS_MAP));
	_NOTIS_F(t->pxls = t->surf->pixels);
	t->s = (point) {t->surf->w, t->surf->h};
	while(++(p.y) < t->s.h && (p.x = -1))
		while (++(p.x) < t->s.w)
			if (t->pxls[p.y * t->s.w + p.x] == WPNS_TEX_BG)
			{
				if ((which_texture_skip =
					dn_check_saved_texture(p, t->tmax, spos, epos)))
					p.x = epos[which_texture_skip - 1].x;
				else
				{
					spos[t->tmax] = (point){p.x, p.y};
					epos[t->tmax] = (point){p.x, p.y};
					while (epos[t->tmax].x < t->s.w
						&& t->pxls[epos[t->tmax].y *
							t->s.w + epos[t->tmax].x] != WPNS_MAP_BG)
						++(epos[t->tmax].x);
					p.x = epos[t->tmax].x;
					while (epos[t->tmax].y < t->s.h
						&& t->pxls[epos[t->tmax].y *
							t->s.w + (p.x - 1)]
							!= WPNS_MAP_BG)
						++(epos[t->tmax].y);
					++(t->tmax);
				}
			}
	_ISZ(point, t->spos, t->tmax);
	_ISZ(point, t->epos, t->tmax);
	int	i = -1;
	while (++i < t->tmax)
	{
		t->spos[i] = spos[i];
		t->epos[i] = epos[i];
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
	_NOTIS_F(dn_init_ck_map(&t->wpns, WPNS_MAP, WPNS_TEX_BG, WPNS_MAP_BG));
	return (true);
}
