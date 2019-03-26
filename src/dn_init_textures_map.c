/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 12:07:04 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int			dn_check_saved_texture(point p, int already_saved_textures,
									point *spos, point *epos)
{
	int	i;

	i = -1;
	while (++i < already_saved_textures)
		if ((p.y >= spos[i].y && p.y < epos[i].y)
		&& (p.x >= spos[i].x && p.x < epos[i].x))
			return (i + 1);
	return (false);
}

static bool	add_copy_pos(t_hinit *h, point *spos, point *epos)
{
	int	i;

	i = -1;
	_ISZ(point, h->t->spos, h->t->tmax);
	_ISZ(point, h->t->epos, h->t->tmax);
	while (++i < h->t->tmax)
	{
		h->t->spos[i] = spos[i];
		h->t->epos[i] = epos[i];
	}
	return (true);
}


static void	add_save_current_texture_pos(point p, point *spos,
										point *epos, t_hinit *h)
{
	spos[h->t->tmax] = (point){p.x, p.y};
	epos[h->t->tmax] = (point){p.x, p.y};
	while (epos[h->t->tmax].x < h->t->s.w
		&& h->t->pxls[epos[h->t->tmax].y * h->t->s.w + epos[h->t->tmax].x]
			!= h->bg_color)
		++(epos[h->t->tmax].x);
	p.x = epos[h->t->tmax].x;
	while (epos[h->t->tmax].y < h->t->s.h
		&& h->t->pxls[epos[h->t->tmax].y * h->t->s.w + (p.x - 1)]
			!= h->bg_color)
		++(epos[h->t->tmax].y);
	++(h->t->tmax);
}

bool		dn_init_ck_map(t_hinit h)
{
	point	p;
	point	spos[h.max_textures];
	point	epos[h.max_textures];
	int		which_texture_skip;

	p.y = -1;
	h.t->tmax = 0;
	which_texture_skip = 0;
	_NOTIS_F(h.t->surf = sdl_load_surface(h.path, IS_FORMAT_SURF));
	_NOTIS_F(h.t->pxls = h.t->surf->pixels);
	h.t->s = (point) {h.t->surf->w, h.t->surf->h};
	while(++(p.y) < h.t->s.h && (p.x = -1))
		while (++(p.x) < h.t->s.w)
			if (h.t->pxls[p.y * h.t->s.w + p.x] == h.ck_color)
			{
				if ((which_texture_skip
					= dn_check_saved_texture(p, h.t->tmax, spos, epos)))
					p.x = epos[which_texture_skip - 1].x;
				else
					add_save_current_texture_pos(p, spos, epos, &h);
			}
	return (add_copy_pos(&h, spos, epos));
}

bool		dn_init_textures_map(t_textures *t)
{
	_NOTIS_F(dn_init_ck_map((t_hinit){&t->wpns, WPNS_MAP, WPNS_TEX_BG,
									WPNS_MAP_BG, WPNS_MAX_TEXTURES}));
	return (true);
}
