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
	int x;
	int y;
	float pitch;

	pitch = 0;
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * MOUSE_SENSIVITY_X / 100.f;
	m->map.player.pitch = clampf(m->map.player.pitch + pitch +
									y * MOUSE_SENSIVITY_Y / 100.f, -5, 5);
	m->map.player.anglesin = sinf(m->map.player.angle);
	m->map.player.anglecos = cosf(m->map.player.angle);
}

t_vertex		get_player_velocity(t_player *p)
{
	t_vertex res;

	ft_bzero(&res, sizeof(res));
	if (p->dir.forward)
	{
		res.x += p->anglecos * 0.1f;
		res.y += p->anglesin * 0.1f;
	}
	if (p->dir.backward)
	{
		res.x -= p->anglecos * 0.1f;
		res.y -= p->anglesin * 0.1f;
	}
	if (p->dir.left)
	{
		res.x += p->anglesin * 0.1f;
		res.y -= p->anglecos * 0.1f;
	}
	if (p->dir.right)
	{
		res.x -= p->anglesin * 0.1f;
		res.y += p->anglecos * 0.1f;
	}
	return (res);
}