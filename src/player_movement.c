/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:29:55 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 15:21:26 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		transform_player(t_map *map)
{	
	if (map->player.is_moving)
	{
		map->player.position.x += map->player.velocity.x * SPEED / 100.f;
		map->player.position.y += map->player.velocity.y * SPEED / 100.f;
	}
	map->player.position.z += map->player.velocity.z;
}

static void		player_vertical_movement(t_main *m)
{
	float		view_h;
	float		nextz;
	t_sector	*cur_sect;

	cur_sect = &m->map.sectors[m->map.player.sector_number];
	view_h = m->map.player.is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	m->map.player.velocity.z -= GRAVITY / 1000.f;
	nextz = m->map.player.position.z + m->map.player.velocity.z;
	if (m->map.player.velocity.z < 0 && nextz < cur_sect->floor_height + view_h)
	{
		m->map.player.position.z = cur_sect->floor_height + view_h;
		m->map.player.velocity.z = 0;
		m->map.player.is_grounded = 1;
	}
	else if (m->map.player.velocity.z > 0 && nextz > cur_sect->ceil_height)
	{
		m->map.player.velocity.z = 0;
		m->map.player.is_grounded = 0;
	}
}

static void		change_sector(t_sector *cur_s, t_sector *next_s,
											int next_s_nbr, t_player *p)
{
	float		hole_low;
	float		hole_high;
	float		eyeheight;

	eyeheight = p->is_crouching ? CROUCHINGHEIGHT : STANDHEIGHT;
	hole_low  = maxf(cur_s->floor_height, next_s->floor_height);
	hole_high = minf(cur_s->ceil_height,  next_s->ceil_height );
	if (hole_high >= p->position.z + HEADMARGIN &&
						hole_low <= p->position.z - eyeheight + KNEEHEIGHT)
	{
		p->sector_number = next_s_nbr;
		p->is_grounded = 0;
	}
	else
	{
		ft_bzero(&p->velocity, sizeof(p->velocity));
		p->is_moving = 0;
	}
	
}

static void		player_horizontal_movement(t_main *m)
{
	t_sector	*sect;
	float		eyeheight;
	int			n_number;
	int			s;

	s = -1;
	sect = &m->map.sectors[m->map.player.sector_number];
	while (++s < sect->number_vertices)
		if (intersects(&m->map.player, sect, s, s + 1))
		{
			n_number = sect->neighbors[s];
			if (n_number >= 0)
			{
				change_sector(sect, &m->map.sectors[n_number], n_number, &m->map.player);
				return ;
			}
			ft_bzero(&m->map.player.velocity, sizeof(m->map.player.velocity));
			m->map.player.is_moving = 0;
		}
}

void			move_player(t_main *m)
{
	t_vertex		velocity;

	m->map.player.velocity.x = 0.f;
	m->map.player.velocity.y = 0.f;
	get_player_direction(m);
	get_player_velocity(&m->map.player);
	player_vertical_movement(m);
	player_horizontal_movement(m);
	transform_player(&m->map);
}
