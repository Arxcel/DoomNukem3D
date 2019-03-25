/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/27 17:20:06 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DOOM_NUKEM_H
# define FT_DOOM_NUKEM_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "ft_sdl.h"
# include "ft_libftu.h"
# include "structure.h"
# include "player.h"
# include "utils.h"
# include "macroses.h"
# include "textures.h"
# include "weapons.h"

# define W 800
# define H 600

# define GRAVITY	1.0f
# define SPEED		10
# define MOUSE_SENSIVITY_X 3
# define MOUSE_SENSIVITY_Y 5

# define NEAR_Z		1e-4f
# define FAR_Z		5
# define NEAR_SIDE	1e-5f
# define FAR_SIDE	20.f
typedef struct		s_main
{
	t_sdl		sdl;
	t_map		map;
	t_textures	tex;
	t_wsys		*wsys;
}					t_main;

void				sdl_hook(t_main *m);
void				sdl_loop(t_main *m);

t_vertex			get_player_velocity(t_main *m);
void				get_player_direction(t_main *m);
void				move_player(t_main *m);
void				drawMinimap(t_img *img, t_map *map);
void				draw_screen(t_main *m);

void				render_sector(t_main *m, t_renderer *renderer, t_render_item const *currentSector);
void				render_wall(t_main *m, t_renderer *renderer, t_wall *wall, t_render_item const *currentSector);

void				init_renderer(t_renderer *r, t_img *img, int n_sectors);
void				free_renderer(t_renderer *r);
void				clamp_point(t_vector *point, t_vertex *i1, t_vertex *i2);
void				draw_line(t_img *img, t_vline *vline);

void				plot_line(t_img *img, t_line *l);
#endif
