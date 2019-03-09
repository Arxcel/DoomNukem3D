/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_renderer.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 10:59:49 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 13:20:35 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_RENDERER_H
# define GRAPHICS_RENDERER_H
# include "structure.h"
# include "ft_sdl.h"

# define NEAR_Z		1e-4f
# define FAR_Z		5
# define NEAR_SIDE	1e-5f
# define FAR_SIDE	20.f

typedef struct		s_render_item
{
	int				sectorno;
	int				limit_x_left;
	int				limit_x_right;
}					t_render_item;

typedef struct		s_renderer
{
	t_render_item	queue[MaxQueue];
	t_render_item	*head;
	t_render_item	*tail;
	int				*rendered_sectors;
	int				*top_limit;
	int				*bottom_limit;
	t_vector		t1;
	t_vector		t2;
	int				w;
	int				h;
}					t_renderer;

typedef struct		s_vline
{
	int				x;
	int				y_top;
	int				y_bottom;
	int				color_top;
	int				color_main;
	int				color_bottom;
}					t_vline;

typedef struct		s_wall
{
	int				x1;
	int				y1[2];
	int				x2;
	int				y2[2];
	t_vertex		scale1;
	t_vertex		scale2;

	float			ceil;
	float			floor;

	int				neighbor;
	float			neighbor_ceil;
	float			neighbor_floor;
	int				neighbor_y1[2];
	int				neighbor_y2[2];

	int				cya;
	int				cyb;
	int				ncya;
	int				ncyb;
}					t_wall;

void		init_renderer(t_renderer *r, t_img *img, int n_sectors);
void		free_renderer(t_renderer *r);
void		clamp_point(t_vector *point, t_vertex *i1, t_vertex *i2);
void		draw_line(t_img *img, t_vline *vline);
void		render_sector(t_img *img, t_map *map, t_renderer *renderer, t_render_item const *currentSector);
void		render_wall(t_img *img, t_renderer *renderer, t_wall *wall, t_render_item const *currentSector);

#endif
