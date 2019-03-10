/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_direction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 13:51:15 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 15:20:57 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void			get_player_direction(t_main *m)
{
	int			x;
	int			y;
	float		pitch;

	pitch = 0;
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * MOUSE_SENSIVITY_X / 100.f;
	m->map.player.anglesin = sinf(m->map.player.angle);
	m->map.player.anglecos = cosf(m->map.player.angle);
	m->map.player.pitch = clampf(m->map.player.pitch + pitch +
									y * MOUSE_SENSIVITY_Y / 100.f, -5, 5);
}

void			get_player_velocity(t_player *p)
{
	p->is_moving = p->dir.forward || p->dir.backward ||
			p->dir.left || p->dir.right;
	if (p->dir.forward)
	{
		p->velocity.x += p->anglecos;
		p->velocity.y += p->anglesin;
	}
	if (p->dir.backward)
	{
		p->velocity.x -= p->anglecos;
		p->velocity.y -= p->anglesin;
	}
	if (p->dir.left)
	{
		p->velocity.x += p->anglesin;
		p->velocity.y -= p->anglecos;
	}
	if (p->dir.right)
	{
		p->velocity.x -= p->anglesin;
		p->velocity.y += p->anglecos;
	}
}
