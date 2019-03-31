/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/12 13:12:32 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/31 14:30:29 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "structure.h"
#include <math.h>

static void		put_line_pixel(t_img *img, t_line *l, int x, int y)
{
	if (x < l->limit_max.x && x > l->limit_min.x &&
						y < l->limit_max.y && y > l->limit_min.y)
		sdl_pixel_put(img, x, y, l->color);
}

static void		plot_line_low(t_img *img, t_line *l)
{
	int x;
	int y;

	y = (int)roundf(l->from.y);
	x = (int)roundf(l->from.x);
	l->dx = (int)roundf(l->to.x) - x;
	l->dy = (int)roundf(l->to.y) - y;
	l->yi = 1;
	if (l->dy < 0)
	{
		l->yi = -1;
		l->dy = -(l->dy);
	}
	l->big_d = 2 * l->dy - l->dx;
	while (x++ != (int)roundf(l->to.x))
	{
		put_line_pixel(img, l, x, y);
		if (l->big_d > 0)
		{
			y += l->yi;
			l->big_d -= 2 * l->dx;
		}
		l->big_d += 2 * l->dy;
	}
}

static void		plot_line_high(t_img *img, t_line *l)
{
	int x;
	int y;

	y = (int)roundf(l->from.y);
	x = (int)roundf(l->from.x);
	l->dx = (int)roundf(l->to.x) - x;
	l->dy = (int)roundf(l->to.y) - y;
	l->xi = 1;
	if (l->dx < 0)
	{
		l->xi = -1;
		l->dx = -(l->dx);
	}
	l->big_d = 2 * l->dx - l->dy;
	while (y++ != (int)roundf(l->to.y))
	{
		put_line_pixel(img, l, x, y);
		if (l->big_d > 0)
		{
			x += l->xi;
			l->big_d -= 2 * l->dy;
		}
		l->big_d += 2 * l->dx;
	}
}

static void		set_direction(t_line *l, t_vertex from, t_vertex to)
{
	l->from = from;
	l->to = to;
}

void			plot_line(t_img *img, t_line *l)
{
	if ((abs(((int)roundf(l->to.y) - (int)roundf(l->from.y)))) <
					(abs(((int)roundf(l->to.x) - (int)roundf(l->from.x)))))
	{
		if (l->from.x > l->to.x)
			set_direction(l, l->to, l->from);
		else
			set_direction(l, l->from, l->to);
		plot_line_low(img, l);
	}
	else
	{
		if (l->from.y > l->to.y)
			set_direction(l, l->to, l->from);
		else
			set_direction(l, l->from, l->to);
		plot_line_high(img, l);
	}
}
