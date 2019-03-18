/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/18 17:48:23 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DOOM_NUKEM_H
# define FT_DOOM_NUKEM_H

# define W 800
# define H 600

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# define GRAVITY	9.87f
# define SPEED		10
# define MOUSE_SENSIVITY_X 3
# define MOUSE_SENSIVITY_Y 5

# define WALLS_BLOCK_SIZE	64
# define WALL_MAX_TEXTURES	8
# define TEXUTRES_MAP		"resources/textures_map.bmp"

# include "ft_sdl.h"
# include "ft_libftu.h"
# include "structure.h"
# include "player.h"
# include "utils.h"
# include "macroses.h"

typedef struct		s_main
{
	t_sdl		sdl;
	t_map		map;
	t_tex_map	tex_map;
}					t_main;

void				sdl_hook(t_main *m);
void				sdl_loop(t_main *m);

void				get_player_direction(t_main *m);
void				move_player(t_main *m);
t_vertex			get_player_velocity(t_player *p);

void				draw_screen(t_img *img, t_map *map);
bool				dn_init_textures_map(t_tex_map *tmap);

#endif
