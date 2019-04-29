/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:31:29 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 11:59:06 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include <stdlib.h>

void		init_renderer(t_renderer *r, t_img *img, int n_sectors)
{
	size_t	x;

	ft_bzero(r, sizeof(t_renderer));
	r->w = img->w;
	r->h = img->h;
	r->rendered_sectors = (int *)malloc(n_sectors * sizeof(int));
	r->top_limit = (int *)malloc(img->w * sizeof(int));
	r->bottom_limit = (int *)malloc(img->w * sizeof(int));
	r->head = r->queue;
	r->tail = r->queue;
	ft_bzero(r->rendered_sectors, n_sectors * sizeof(int));
	ft_bzero(r->top_limit, img->w * sizeof(int));
	x = -1;
	while (++x < img->w)
		r->bottom_limit[x] = img->h - 1;
}

void		free_renderer(t_renderer *renderer)
{
	free(renderer->rendered_sectors);
	free(renderer->top_limit);
	free(renderer->bottom_limit);
}

void		draw_line(t_main *m, t_wall *w, t_vline *v, t_interp *ty)
{
	int				y;
	int				*pix;
	SDL_Surface		*current;

	current = m->tex.t.textures[v->texture_id];
	pix = current->pixels;
	v->y_top = clampf(v->y_top, 0, m->sdl.img.h - 1);
	v->y_bottom = clampf(v->y_bottom, 0, m->sdl.img.h - 1);
	y = v->y_top;
	while (++y <= v->y_bottom)
	{
		w->txty = interp_next(ty);
		sdl_pixel_put(&m->sdl.img, v->x, y,
			c_darken(pix[w->txtx % current->w +
							(w->txty % current->h) * current->w], w->lz));
	}
}

void		clamp_point(t_vertex *point, t_vertex *i1, t_vertex *i2)
{
	if (i1->y > 0)
	{
		point->x = i1->x;
		point->y = i1->y;
	}
	else if (i2->y > 0)
	{
		point->x = i2->x;
		point->y = i2->y;
	}
}
