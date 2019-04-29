/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_collisions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/20 18:33:51 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 17:09:28 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		check_total_active_weapon(t_player *p)
{
	if (p->stats.total_active_weapon <= 3)
		p->stats.total_active_weapon++;
}

static void		sprite_collision_ligic2(t_main *m, t_player *p, t_sprite *s)
{
	if (s->texture == 6)
	{
		p->stats.armor += 50;
		Mix_PlayChannel(-1, m->music.snd[7], 0);
	}
	else if (s->texture == 7)
	{
		Mix_PlayChannel(-1, m->music.snd[10], 0);
		m->victory = true;
	}
	else if (s->texture == 8 || s->texture == 9)
	{
		p->stats.active_weapon = 2;
		check_total_active_weapon(p);
		Mix_PlayChannel(-1, m->music.snd[7], 0);
	}
	else if (s->texture == 9)
	{
		p->stats.active_weapon = 3;
		check_total_active_weapon(p);
		Mix_PlayChannel(-1, m->music.snd[7], 0);
	}
	s->is_active = false;
	m->greenify = true;
}

static void		sprite_collision_ligic(t_main *m, t_player *p, t_sprite *s)
{
	if (s->texture == 0)
	{
		p->stats.ammo += 10;
		Mix_PlayChannel(-1, m->music.snd[0], 0);
	}
	else if (s->texture == 2)
	{
		p->stats.armor += 10;
		Mix_PlayChannel(-1, m->music.snd[7], 0);
	}
	else if (s->texture == 3 || s->texture == 5)
	{
		p->stats.hp += 50;
		Mix_PlayChannel(-1, m->music.snd[2], 0);
	}
	else if (s->texture == 4)
	{
		p->stats.rockets += 50;
		Mix_PlayChannel(-1, m->music.snd[0], 0);
	}
	sprite_collision_ligic2(m, &m->map.player, s);
}

void			calc_sprite_collisions(t_main *m)
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
		if (d > 1)
			continue ;
		if (s->texture == 0 || s->texture == 2 || s->texture == 3 ||
			s->texture == 4 || s->texture == 5 || s->texture == 6 ||
			s->texture == 7 || s->texture == 8 || s->texture == 9)
			sprite_collision_ligic(m, &m->map.player, s);
	}
}
