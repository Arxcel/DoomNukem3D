/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_direction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:51:15 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/21 18:55:05 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		find_angle(t_main *m, int sign)
{
	while (1)
	{
		m->map.player.anglesin = sinf(m->map.player.angle);
		m->map.player.anglecos = cosf(m->map.player.angle);
		if (!(fabs(m->map.player.anglesin) >= 0.01 &&
								fabs(m->map.player.anglesin) <= 0.25) &&
			!(fabs(m->map.player.anglecos) >= 0.01 &&
										fabs(m->map.player.anglecos) <= 0.25))
			break ;
		m->map.player.angle += 0.007 * sign;
	}
}

void			get_player_direction(t_main *m)
{
	int		x;
	int		y;
	float	pitch;

	pitch = 0;
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * MOUSE_SENSIVITY_X / 100.f;
	if (m->map.player.angle >= 2 * M_PI)
		m->map.player.angle = 0;
	else if (m->map.player.angle < 0)
		m->map.player.angle = 2 * M_PI;
	m->map.player.pitch = clampf(m->map.player.pitch + pitch +
									y * MOUSE_SENSIVITY_Y / 100.f, -5, 5);
	find_angle(m, x > 0 ? 1 : -1);
}

static void		calc_velocity(t_vertex *res, t_main *m, int speed)
{
	if (m->map.player.dir.forward)
	{
		res->x += m->map.player.anglecos * m->delta_time * speed;
		res->y += m->map.player.anglesin * m->delta_time * speed;
	}
	if (m->map.player.dir.backward)
	{
		res->x -= m->map.player.anglecos * m->delta_time * speed;
		res->y -= m->map.player.anglesin * m->delta_time * speed;
	}
	if (m->map.player.dir.left)
	{
		res->x += m->map.player.anglesin * m->delta_time * speed;
		res->y -= m->map.player.anglecos * m->delta_time * speed;
	}
	if (m->map.player.dir.right)
	{
		res->x -= m->map.player.anglesin * m->delta_time * speed;
		res->y += m->map.player.anglecos * m->delta_time * speed;
	}
}

t_vertex		get_player_velocity(t_main *m)
{
	t_vertex	res;
	int			speed;

	speed = m->map.player.is_running ? RUN_SPEED : SPEED;
	ft_bzero(&res, sizeof(res));
	calc_velocity(&res, m, speed);
	return (res);
}
