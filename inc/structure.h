/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:32:42 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:32:42 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURE_H
# define STRUCTURE_H
# include "ft_sdl.h"
# include <ctype.h>

enum
{
	MaxQueue = 32
};

enum	e_bool {false, true} __attribute__((packed));

# define _BOOL  typedef enum e_bool	bool

_BOOL;

typedef struct		s_vector
{
	float			x;
	float			y;
	float			z;
}					t_vector;

typedef struct		s_vertex
{
	float			x;
	float			y;
}					t_vertex;

typedef struct		s_point
{
	int				x;
	int				y;
}					t_point;

typedef struct		s_line
{
	t_vertex		from;
	t_vertex		to;
	int				dx;
	int				dy;
	int				big_d;
	int				xi;
	int				yi;
	int				color;
	t_vertex		limit_min;
	t_vertex		limit_max;
}					t_line;

typedef struct	s_dir
{
	bool	forward;
	bool	backward;
	bool	right;
	bool	left;
}				t_dir;

typedef struct		s_sector
{
	float			floor_height;
	float			ceil_height;
	short			*neighbors;
	t_vertex		*vertices;
	size_t			number_vertices;
	t_vertex		min;
	t_vertex		max;
}					t_sector;

typedef struct		s_sprite
{
	t_vector		position;
	float			angle;
	float			pitch;
	float			anglesin;
	float			anglecos;
	size_t			sector_number;
}					t_sprite;

typedef struct		s_player
{
	t_vector		position;
	t_vector		velocity;
	float			angle;
	float			pitch;
	float			anglesin;
	float			anglecos;
	size_t			sector_number;

	bool			is_moving;
	bool			is_crouching;
	bool			is_standing;
	bool			is_falling;

	t_dir			dir;
}					t_player;

typedef struct		s_map
{
	t_vertex		*vertices;
	t_sector		*sectors;
	t_sprite		*sprites;
	t_player		player;
	size_t			number_vertices;
	size_t			number_sectors;
	size_t			number_enemies;
	size_t			number_sptites;
}					t_map;

/*
**	start or rendering help structures
*/
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
	t_vertex		t1;
	t_vertex		t2;
	int				w;
	int				h;
}					t_renderer;

typedef struct		s_vline
{
	int				x;
	int				y_top;
	int				y_bottom;
	int				texture_id;
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

	int				ya;
	int				yb;
	int				nya;
	int				nyb;

	int				cya;
	int				cyb;
	int				ncya;
	int				ncyb;

	int				u0;
	int				u1;
	
	int				txtx;
	int				txty;

	int				upper_id;
	int				lower_id;
	int				solid_id;
	int				floor_id;
	int				ceil_id;

}					t_wall;

typedef struct s_interpolator
{
	int				current;
	int				step;
	int				target_length;
	int				basic_length;
	int				temp;
}					t_interp;

typedef struct	s_texture_blocks
{
	SDL_Surface	**textures;
	size_t		numTextures;
}				t_tblocks;

typedef struct		s_textures
{
	t_tblocks		t;
}					t_textures;

typedef struct		s_main
{
	t_sdl		sdl;
	t_map		map;
	t_textures	tex;
	float		prev_time;
	float		curr_time;
	float		delta_time;
}					t_main;

#endif
