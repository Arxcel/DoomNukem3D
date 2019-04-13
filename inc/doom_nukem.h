/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 16:03:46 by vkozlov          ###   ########.fr       */
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
# include "utils.h"

# define W 1280
# define H 720

# define GRAVITY				1.0f
# define SPEED					10
# define MOUSE_SENSIVITY_X		3
# define MOUSE_SENSIVITY_Y		5

# define STANDHEIGHT			6
# define CROUCHINGHEIGHT		2.5
# define HEADMARGIN				1
# define KNEEHEIGHT				2
# define HFOV					0.73f
# define VFOV					.2f

# define NEAR_Z		1e-4f
# define FAR_Z		5
# define NEAR_SIDE	1e-5f
# define FAR_SIDE	20.f

# ifdef __APPLE__
#  define WPNS_TEX_BG	0xff00ffff
#  define WPNS_MAP_BG	0xff95b1c8
#  define PIXEL_FORMAT	SDL_PIXELFORMAT_ARGB8888
#  define IS_FORMAT_SURF	0
# endif

# ifdef __linux__
#  define WPNS_TEX_BG	0xffff
#  define WPNS_MAP_BG	0x95b1c8
#  define PIXEL_FORMAT	SDL_PIXELFORMAT_RGB888
#  define IS_FORMAT_SURF	1
# endif

void				sdl_hook(t_main *m);
void				sdl_loop(t_main *m);

void				do_perspective(t_renderer *renderer, t_wall *w,
													int width, int height);
t_vertex			reverse_perspective(t_main *m, int x, int y, float height);

t_vertex			get_player_velocity(t_main *m);
void				get_player_direction(t_main *m);
void				move_player(t_main *m);
void				draw_minimap(t_main *m);
void				draw_screen(t_main *m);

void				render_sector(t_main *m, t_renderer *renderer,
								t_render_item const *current_sector);
void				render_wall(t_main *m, t_renderer *renderer, t_wall *wall,
								t_render_item const *current_sector);

void				init_renderer(t_renderer *r, t_img *img, int n_sectors);
void				free_renderer(t_renderer *r);
void				clamp_point(t_vertex *point, t_vertex *i1, t_vertex *i2);
void				draw_line(t_main *m, t_wall *w, t_vline *v, t_interp *ty);
double				cast_ray_2line(t_vertex ray_origin,
					t_vertex ray_direction, t_vertex point1, t_vertex point2);
void				plot_line(t_img *img, t_line *l);

void				load_textures(t_main *m);
void				clear_textures(t_main *m);
void				setup_wall_texture(t_main *m, t_wall *w,
													int wall, t_pt verical);

t_vertex			calculate_edges(t_player *player, t_vertex *vertex);
void				clamp_edges_with_player_view(t_renderer *r, t_wall *w);

#endif
