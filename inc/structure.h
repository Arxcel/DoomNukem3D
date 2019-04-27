/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structure.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:59:04 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 11:30:09 by vkozlov          ###   ########.fr       */
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
}					t_pt;

typedef struct		s_segment
{
	int				a;
	int				b;
}					t_seg;

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

typedef struct		s_dir
{
	bool			forward;
	bool			backward;
	bool			right;
	bool			left;
}					t_dir;

typedef struct		s_sector
{
	float			floor_height;
	float			ceil_height;
	short			*neighbors;
	t_vertex		*vertices;
	int				*textures;
	size_t			number_vertices;
	t_vertex		min;
	t_vertex		max;
}					t_sector;

typedef struct		s_sprite
{
	t_vector		position;
	unsigned		texture;
	bool			is_active;
	unsigned		w;
	unsigned		h;
	int				sprite_ceil;
	int				sprite_floor;
}					t_sprite;

typedef struct		s_stats
{
	int				ammo;
	int				rockets;
	int				hp;
	int				armor;
	int				active_weapon;
}					t_stats;

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
	bool			is_running;

	t_dir			dir;

	t_stats			stats;
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
	size_t			number_sprites;
	float			ligntness;
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
	t_vertex		t1;
	t_vertex		t2;

	int				x1;
	t_seg			y1;
	int				x2;
	t_seg			y2;
	t_vertex		scale1;
	t_vertex		scale2;

	float			ceil;
	float			floor;

	int				neighbor;
	float			n_ceil;
	float			n_floor;
	t_seg			n_y1;
	t_seg			n_y2;

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

	float			lz;
}					t_wall;

typedef struct		s_interpolator
{
	int				current;
	int				step;
	int				target_length;
	int				basic_length;
	int				temp;
}					t_interp;

typedef struct		s_texture_blocks
{
	SDL_Surface		**textures;
	size_t			num_textures;
}					t_tblocks;

typedef struct		s_textures
{
	t_tblocks		t;
	t_tblocks		s;
}					t_textures;

typedef struct		s_music
{
	Mix_Chunk		**snd;
	size_t			num_sounds;
}					t_music;

typedef struct		s_hud
{
	TTF_Font		*font;
	SDL_Surface		*surface_message;
	SDL_Texture		*message;
	SDL_Rect		message_rect;
	SDL_Surface		*surface_hud;
	SDL_Texture		*hud;
	SDL_RWops		*font_rwops;
	char			*font_source;
	SDL_Rect		hud_rect;

}					t_hud;

typedef struct		s_main
{
	t_sdl			sdl;
	t_music			music;
	t_map			map;
	t_textures		tex;
	t_hud			hud;
	float			prev_time;
	float			curr_time;
	float			delta_time;
}					t_main;

#endif
