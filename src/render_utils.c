/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 12:31:29 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:00:07 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include <stdlib.h>

void		init_renderer(t_renderer *r, t_img *img, int n_sectors)
{
	int x;

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

void		draw_line(t_img *img, t_vline *vline)
{
	int y;

	vline->y_top = clampf(vline->y_top, 0, img->h - 1);
	vline->y_bottom = clampf(vline->y_bottom, 0, img->h - 1);
	if (vline->y_bottom == vline->y_top)
		sdl_pixel_put(img, vline->x, vline->y_top, vline->color_main);
	else if (vline->y_bottom > vline->y_top)
	{
		sdl_pixel_put(img, vline->x, vline->y_top, vline->color_top);
		y = vline->y_top;
		while (++y < vline->y_bottom)
			sdl_pixel_put(img, vline->x, y, vline->color_main);
		sdl_pixel_put(img, vline->x, vline->y_bottom, vline->color_bottom);
	}
}

void		draw_line2(t_main *m, t_vline *vline, t_interp ty, unsigned txtx)
{
	int y;
	int *pix;
	int id;

	id = vline->texture_id % m->tex.t.numTextures;
	pix = m->tex.t.textures[id]->pixels;
	vline->y_top = clampf(vline->y_top, 0, m->sdl.img.h - 1);
	vline->y_bottom = clampf(vline->y_bottom, 0, m->sdl.img.h - 1);
	y = vline->y_top;
	while (++y <= vline->y_bottom)
	{
		unsigned txty = interp_next(&ty);
		sdl_pixel_put(&m->sdl.img, vline->x, y, pix[txtx % m->tex.t.textures[id]->w + (txty % m->tex.t.textures[id]->h) * m->tex.t.textures[id]->w]);
	}

	// pix += y1 * W2 + x;
    // for(int y = y1; y <= y2; ++y)
    // {
    //     unsigned txty = Scaler_Next(&ty);
    //     *pix = t->texture[txtx % 1024][txty % 1024];
    //     pix += W2;
    // }
}

void		clamp_point(t_vector *point, t_vertex *i1, t_vertex *i2)
{
	if (i1->y > 0)
	{
		point->x = i1->x;
		point->z = i1->y;
	}
	else if (i2->y > 0)
	{
		point->x = i2->x;
		point->z = i2->y;
	}
}
