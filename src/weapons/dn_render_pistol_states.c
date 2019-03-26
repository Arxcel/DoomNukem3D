/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_render_pistol_states.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 12:27:26 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 18:30:35 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	pidle(t_weapon *w, t_tmap *t, t_img *i)
{
	Uint32	curr_pxl;
	point	p;

	p.y = w->spos[0].y - 1;
	while (++(p.y) < w->epos[0].y && (p.x = w->spos[0].x - 1))
		while (++(p.x) < w->epos[0].x)
		{
			curr_pxl = t->pxls[p.y * t->s.w + p.x];
			if (curr_pxl != WPNS_TEX_BG)
				sdl_pixel_put(i, PSHIFT_X, PSHIFT_Y, curr_pxl);
		}
}

void	pshot(t_weapon *w, t_tmap *t, t_img *i)
{
	if (w)
		;
	if (t)
		;
	if (i)
		;
	printf("pistol shot state\n");
}

void	preload(t_weapon *w, t_tmap *t, t_img *i)
{
	if (w)
		;
	if (t)
		;
	printf("pistol reload state\n");
}

void	pdraw(t_weapon *w, t_tmap *t, t_img *i)
{
	if (w)
		;
	if (t)
		;
	if (i)
		;
	printf("pistol draw state\n");
}

void	phide(t_weapon *w, t_tmap *t, t_img *i)
{
	if (w)
		;
	if (t)
		;
	if (i)
		;
	printf("pistol hide state\n");
}
