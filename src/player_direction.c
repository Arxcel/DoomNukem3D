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


void	get_player_direction(t_main *m)
{
	int			x;
	int			y;
	float		pitch;

	pitch = 0;
	SDL_GetRelativeMouseState(&x, &y);
	m->map.player.angle += x * 0.03f;
	m->map.player.anglesin = sinf(m->map.player.angle);
	m->map.player.anglecos = cosf(m->map.player.angle);
	pitch = clampf(pitch + y * 0.05f, -5, 5);
	m->map.player.pitch = clampf(m->map.player.pitch, -5, 5);
	printf("%f\n", m->map.player.pitch);
}
