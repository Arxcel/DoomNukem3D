/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/14 11:31:25 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include "map_parser.h"

static void			calc_delta_time(t_main *m)
{
	m->prev_time = m->curr_time;
	m->curr_time = SDL_GetPerformanceCounter();
	m->delta_time += (m->curr_time - m->prev_time) /
									(float)(SDL_GetPerformanceFrequency());
}

static float		yaw(t_main *m, float y, float z)
{
	return (m->map.player.pitch * z + y);
}

void renderSprites(t_main *m)
{
	float	abx;
	float	aby;
	float	dist;

	abx = (m->map.player.position.x - m->map.sprites[0].position.x);
	aby = (m->map.player.position.y - m->map.sprites[0].position.y);
	dist = (float)sqrt(abx * abx + aby * aby);
	
	float v1_x_loc;
	float v1_y_loc;
	float v2_x_loc;
	float v2_y_loc;

	float vx1;
	float vy1;
	float vx2;
	float vy2;

	// find_sprite_end(main, DRS.dist);
	v1_x_loc = (m->map.sprites[0].position.x - m->map.player.position.x) / dist;
	v1_y_loc = (m->map.sprites[0].position.y - m->map.player.position.y) / dist;
	v2_x_loc = -v1_y_loc;
	v2_y_loc = v1_x_loc;
	vx1 = (m->map.sprites[0].position.x - m->map.player.position.x);
	vy1 = (m->map.sprites[0].position.y - m->map.player.position.y);
	vx2 = (v2_x_loc * 1) + m->map.sprites[0].position.x - m->map.player.position.x;
	vy2 = (v2_y_loc * 1) + m->map.sprites[0].position.y - m->map.player.position.y;

	//	transformation_formulas
	float pcos;
	float psin;
	float tx1;
	float tz1;
	float tx2;
	float tz2;

	pcos = m->map.player.anglecos;
	psin = m->map.player.anglesin;
	tx1 = vx1 * psin - vy1 * pcos;
	tz1 = vx1 * pcos + vy1 * psin;
	tx2 = vx2 * psin - vy2 * pcos;
	tz2 = vx2 * pcos + vy2 * psin;
	if (tz1 <= 0 && tz2 <= 0)
		return ;

	//	perspective_transformation
	float x_scale1;
	float y_scale1;
	float x_scale2;
	float y_scale2;
	int x1;
	int x2;
	x_scale1 = HFOV / tz1 * W;
	y_scale1 = VFOV / tz1 * H;
	x1 = W / 2 - (int)(tx1 * x_scale1);
	x_scale2 = HFOV / tz2 * W;
	y_scale2 = VFOV / tz2 * H;
	x2 = W / 2 - (int)(tx2 * x_scale2);

	//	find_sprite_visual
	float y_ceil;
	float y_floor;
	float y1_a;
	float y1_b;
	float y2_a;
	float y2_b;

	int	begin_x;
	int end_x;

	y_ceil = m->map.sprites[0].position.z + m->map.sprites[0].h - m->map.player.position.z;
	y_floor = m->map.sprites[0].position.z - m->map.player.position.z;
	y1_a = H / 2 - (int)(yaw(m, y_ceil, tz1) * y_scale1);
	y1_b = H / 2 - (int)(yaw(m, y_floor, tz1) * y_scale1);
	y2_a = H / 2 - (int)(yaw(m, y_ceil, tz2) * y_scale2);
	y2_b = H / 2 - (int)(yaw(m, y_floor, tz2) * y_scale2);
	begin_x = x1;
	end_x = begin_x + m->map.sprites[0].w * 100 / dist;
	if (end_x < 0)
		end_x = 0;

// void	draw_key_sprite(t_main_data *main, int key)
	int		x;
	x = begin_x;
	while (x < end_x)
	{
		if (x < W && x > 0)
		{
			if (x > begin_x - 1)
			{
				int y1;
				int y2;
				y1 = (370 + dist * 0.3) - m->map.player.pitch * 160;
				y2 = y1 + (end_x - begin_x);
				int		y;
				if (y2 > y1)
				{
					y = y1;
					while (y <= y2)
					{
						if (y >= y1)
							if (y < H && 0 < y)
								sdl_pixel_put(&m->sdl.img, x, y, 0xFF0000);
						y++;
					}
				}
			}
		}
		x++;
	}
}

void				sdl_loop(t_main *m)
{
	while (m->sdl.running)
	{
		calc_delta_time(m);
		sdl_hook(m);
		
		if (m->delta_time > 0.016f)
		{
			draw_screen(m);
			draw_minimap(m);
			renderSprites(m);
			sdl_put_image(&m->sdl);
			move_player(m);
			m->delta_time = 0.f;
		}
	}
}

static void initSprite(t_main *m)
{
	m->map.sprites = (t_sprite*)malloc(sizeof(t_sprite) * 1);
	m->map.number_sptites = 1;
	// m->map.sprites[0].position = (t_vector){75.0,174.0,0.0};
	m->map.sprites[0].position = (t_vector){15.0,15.0,0.0};
	m->map.sprites[0].w = 30;
	m->map.sprites[0].h = 20;
	m->map.sprites[0].isActive = true;
	m->map.sprites[0].texture = 0;
	m->map.sprites[0].sprite_ceil = 30;
	m->map.sprites[0].sprite_floor = 0;

	// m->map.player.angle = 2;
	// m->map.player.position = (t_vector){75.0,147.0,0.0};

}

int					main(int ac, char **av)
{
	t_main			m;
 
	(void)av;
	ft_bzero(&m, sizeof(t_main));
	load_textures_snd(&m);
	parser(&m.map, av[1]);
	m.sdl.win_w = W;
	m.sdl.win_h = H;
	sdl_init(&m.sdl);
	initSprite(&m);
	sdl_loop(&m);
	clear_textures(&m);
	remove_data(&m.map);
	Mix_CloseAudio();
	SDL_Quit();
	return (0);
}
