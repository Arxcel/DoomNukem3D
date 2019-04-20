/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_collisions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 18:33:51 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 19:19:46 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		sprite_collision_ligic(t_player *p, t_sprite *s)
{
	if (s->texture == 0)
		p->stats.ammo += 10;
	else if (s->texture == 2)
		p->stats.armor += 10;
	else if (s->texture == 3)
		p->stats.hp += 50;
	else if (s->texture == 4)
		p->stats.rockets += 2;
	s->is_active = false;
}

void			calk_sprite_collisions(t_main *m)
{
	int			i;
	float		d;
	t_sprite	*s;

	i = -1;
	while (++i < m->map.number_sprites)
	{
		if (!m->map.sprites[i].is_active)
			continue;
		s = &m->map.sprites[i];
		d = calc_distance(m->map.player.position, s->position);
		if (d > 0.7)
			continue ;
		if (s->texture == 0 || s->texture == 2 ||
							s->texture == 3 || s->texture == 4)
			sprite_collision_ligic(&m->map.player, s);
	}
}
