/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 18:35:50 by tmaluh           ###   ########.fr       */
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

static bool	add_scale_surface(t_hinit *h, point scale)
{
	SDL_Surface		*temp;
	SDL_Rect		dst_rect;

	_NOTIS_F(temp = sdl_load_surface(WPNS_MAP, 1));
	dst_rect = (SDL_Rect){0, 0, temp->w * scale.w, temp->h * scale.h};
	h->t->surf = SDL_CreateRGBSurfaceWithFormat(0, dst_rect.w, dst_rect.h, 32,
												SDL_PIXELFORMAT_ARGB8888);
	_IS(SDL_BlitScaled(temp, NULL, h->t->surf, &dst_rect) < 0);
	_NOTIS_F(h->t->pxls = h->t->surf->pixels);
	h->t->s = (point) {h->t->surf->w, h->t->surf->h};
	SDL_FreeSurface(temp);
	return (true);
}

bool		dn_init_ck_map(t_hinit h, point scale)
{
	point		p;
	point		spos[h.max_textures];
	point		epos[h.max_textures];
	int			which_texture_skip;

	p.y = -1;
	h.t->tmax = 0;
	which_texture_skip = 0;
	_NOTIS_F(add_scale_surface(&h, scale));
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
