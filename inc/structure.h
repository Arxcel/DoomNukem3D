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
# include "enum.h"
# include "ft_sdl.h"
# include <ctype.h>

/*
**	"Undefined Behaviour" is can happens in C if use this union below.
**	But why not ? =)
*/
typedef union
{
	struct
	{
		int	x;
		int	y;
	};
	struct
	{
		int	w;
		int	h;
	};
}					t_upoint;

# define _POINT	typedef t_upoint	point

_POINT;

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
**	spos - start positions of textures on texture map
**	epos - end positions of textures on texture map
**	w & h - is surf(pxls), but not of each texture on texture map
*/
typedef struct	s_textures_map
{
	SDL_Surface	*surf;
	int			*pxls;
	point		s;
	point		*spos;
	point		*epos;
	int			tmax;
}				t_tmap;


typedef struct		s_textures
{
	t_tmap	walls;
	t_tmap	wpns;
}					t_textures;

typedef struct	s_init_helper
{
	t_tmap	*t;
	char	*path;
	Uint32	ck_color;
	Uint32	bg_color;
	int		max_textures;
}				t_hinit;

/*
**	End of Staff for work with textures.
*/

#endif
