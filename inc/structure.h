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
# include <ctype.h>

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

typedef struct	s_dir
{
	short	forward;
	short	backward;
	short	right;
	short	left;
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

	short			is_moving;
	short			is_crouching;
	short			is_standing;
	short			is_falling;

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




#endif
