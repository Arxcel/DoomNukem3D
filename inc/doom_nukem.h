/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doom_nukem.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 19:46:22 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DOOM_NUKEM_H
# define FT_DOOM_NUKEM_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "zip.h"
# include "ft_sdl.h"
# include "ft_libftu.h"
# include "structure.h"
# include "utils.h"
# include "json.h"
# include "json-builder.h"
# include <unistd.h>

# define W 			1200
# define H 			800

# define SPEED					5
# define RUN_SPEED				10
# define MOUSE_SENSIVITY_X		1
# define MOUSE_SENSIVITY_Y		2
# define LIFT_SPEED				1

# define STANDHEIGHT			6
# define CROUCHINGHEIGHT		2.5
# define HEADMARGIN				1
# define KNEEHEIGHT				2
# define HFOV					0.73f
# define VFOV					.2f

# define RESOURCES				"./assets.kek"
# define RESOURCES_PASS			"lol"

# define NEAR_Z		1e-4f
# define FAR_Z		5
# define NEAR_SIDE	1e-5f
# define FAR_SIDE	20.f

# define SPRITE_SCALE_X		100
# define SPRITE_SCALE_Y		100

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

void				do_perspective(t_wall *w, int width, int height);

t_vertex			reverse_perspective(t_main *m, int x, int y, float height);

t_vertex			get_player_velocity(t_main *m);
void				get_player_direction(t_main *m);
void				move_player(t_main *m);
void				draw_minimap(t_main *m);
void				draw_screen(t_main *m);
unsigned int		c_greenify(unsigned int color, float green);
unsigned int		c_darken(unsigned int color, float darkness);
unsigned int		c_rgb(unsigned int r, unsigned int g, unsigned int b);
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
void				load_sprites(t_main *m);
void				unload_textures_and_sprites(t_main *m);
void				setup_wall_texture(t_main *m, t_wall *w,
													int wall, t_pt verical);
void				setup_sprite_texture(t_main *m, t_wall *w, int wall);
t_vertex			calculate_edges(t_player *player, t_vertex *vertex);
t_vertex			calculate_edges2(t_player *player, t_vertex *vertex);
void				clamp_edges_with_player_view(t_wall *w);
void				draw_local_wall(t_main *m, t_wall *wall,
													t_renderer *r, int x);
void				calc_sprites(t_player *p, t_vector sprite_pos,
												t_wall *wall, float dist);
void				draw_sprites(t_main *m);
void				draw_sprite_line(t_main *m, t_wall *w,
													t_vline *v, t_interp *ty);
void				calc_sprite_collisions(t_main *m);
float				calc_distance(t_vector v1, t_vector v2);
void				sort_sprites(t_main *m, int *order, float *dist);
void				load_hud(t_main *m);
void				unload_hud(t_main *m);
void				load_sounds(t_main *m);
void				unload_sounds(t_main *m);
void				draw_hud(t_main *m);
void				draw_text(t_main *m, char *text, int x, int y);
void				draw_gun(t_main *m);

void				draw_green(t_main *m);
void				calc_green_time(t_main *m);
void				shoot(t_main *m);
void				standard_mode(t_main *m);
void				menu_mode(t_main *m);
void				victory_mode(t_main *m);
void				draw_menu(t_main *m);
void				handle_menu(t_main *m, int key);
void				do_general_opt(t_main *m);
void				do_select_opt(t_main *m);
void				draw_victory(t_main *m);

int					parser(t_map *map, char *file_name);
int					remove_data(t_map *map);
int					remove_values(t_map *map, json_value *value);
int					player_field(t_map *map, json_value *value);
int					sector_field(t_map *map, json_value *value);
int					sprite_field(t_map *map, json_value *value);
int					count_vertices(t_map *map, json_value *value);
void				unload_resources(t_main *m);
void				load_resources(t_main *m);
int					init_map_editor(t_main *main);
int					map_editor_loop(t_main *m);
int					serialize_map(t_main *m, t_map_editor *e);
int					editor_clear_sdl(t_sdl *sdl);
int					intersect(t_editor_wall wall, t_dot cur);
int					line(t_editor_wall wall, t_main *main);
void				init_sectors(t_map_editor *e);
void				init_sprites(t_map_editor *e);
void				create_text_menu(t_main *m, t_text *menu);
void				update_text(t_main *m, t_text *menu, int i, int str);
void				update_text_color(t_main *m, t_text *menu, int i);
int					create_text(t_main *m, t_text *menu, int i, SDL_Color fg);
void				update_all_menu(t_main *m, t_map_editor *e);
void				create_sprite_menu(t_main *m, t_text *sprite_menu);
void				update_sprite_menu(t_main *m, t_map_editor *e);

void				print_vector(t_editor_wall wall, t_main *main);
int					draw(t_main *m, t_map_editor *e);
int					draw_circle(int color, t_main *m, t_vector position);

int					pnpoly(int num_walls, t_editor_wall *walls, t_dot dot);
int					check_intersection(t_map_editor *e, t_dot mouse);

int					player_save_keys(t_main *m, t_map_editor *e);
int					arrow_keys(t_main *m, t_map_editor *e);
int					left_arrow_key(t_main *m, t_map_editor *e);

void				shift_left(t_map_editor *e);

int					remove_text_menu(t_map_editor *e);
int					init_gun_surface(t_main *m);
void				calc_lifts(t_main *m);
void				handle_menu(t_main *m, int key);
void				check_menu_active_option(t_main *m, int check);
json_value			*serialize_sectors(t_map_editor *e, json_value **vert);
int					write_map_to_file(char *buf, const char *filename);
int					up_arrow_key(t_main *m, t_map_editor *e);
int					down_arrow_key(t_main *m, t_map_editor *e);
#endif
