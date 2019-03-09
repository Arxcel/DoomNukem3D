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

#include "graphics_renderer.h"
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

void		clamp_point(t_vector *point, t_vertex *i1, t_vertex *i2)
{
	if (i1->y > 0)
	{
		point->x = i1->x;
		point->z = i1->y;
	}
	else
	{
		point->x = i2->x;
		point->z = i2->y;
	}
}
