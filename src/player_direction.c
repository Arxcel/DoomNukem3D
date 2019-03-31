/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_direction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:51:15 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/31 14:00:58 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

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
	m->map.player.anglesin = sinf(m->map.player.angle);
	m->map.player.anglecos = cosf(m->map.player.angle);
}

t_vertex		get_player_velocity(t_main *m)
{
	t_vertex res;

	ft_bzero(&res, sizeof(res));
	if (m->map.player.dir.forward)
	{
		res.x += m->map.player.anglecos * m->delta_time * SPEED;
		res.y += m->map.player.anglesin * m->delta_time * SPEED;
	}
	if (m->map.player.dir.backward)
	{
		res.x -= m->map.player.anglecos * m->delta_time * SPEED;
		res.y -= m->map.player.anglesin * m->delta_time * SPEED;
	}
	if (m->map.player.dir.left)
	{
		res.x += m->map.player.anglesin * m->delta_time * SPEED;
		res.y -= m->map.player.anglecos * m->delta_time * SPEED;
	}
	if (m->map.player.dir.right)
	{
		res.x -= m->map.player.anglesin * m->delta_time * SPEED;
		res.y += m->map.player.anglecos * m->delta_time * SPEED;
	}
	return (res);
}
