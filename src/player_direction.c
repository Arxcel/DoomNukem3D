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

static void	get_direction(t_player *p, t_vertex *dir)
{
	if (p->dir == Forward)
	{
		dir->x += p->anglecos * 0.1f;
		dir->y += p->anglesin * 0.1f;
	}
	if (p->dir == Backward)
	{
		dir->x -= p->anglecos * 0.1f;
		dir->y -= p->anglesin * 0.1f;
	}
	if (p->dir == Left)
	{
		dir->x += p->anglesin * 0.1f;
		dir->y -= p->anglecos * 0.1f;
	}
	if (p->dir == Right)
	{
		dir->x -= p->anglesin * 0.1f;
		dir->y += p->anglecos * 0.1f;
	}
}

t_vertex	get_player_direction(t_main *m)
{
	int			x;
	int			y;
	float		pitch;
	t_vertex	move_dir;

	pitch = 0;
	ft_bzero(&move_dir, sizeof(t_vertex));
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * 0.03f;
	pitch = clampf(pitch + y * 0.05f, -5, 5);
	m->map.player.pitch += pitch - m->map.player.velocity.z * 0.1f;
	get_direction(&m->map.player, &move_dir);
	transform_player(&m->map);
	return (move_dir);
}
