/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_keys2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 19:34:18 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 20:48:39 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int				left_arrow_key(t_main *m, t_map_editor *e)
{
	if (m->sdl.e.key.keysym.sym != SDLK_LEFT ||
								(e->n && e->sectors[e->n].num_walls < 2))
		return (1);
	if (!e->n && e->sectors[e->n].num_walls < 0)
		return (1);
	e->sectors[e->n].walls[e->sectors[e->n].num_walls - 1].texture = 0;
	e->sectors[e->n].num_walls--;
	if ((e->n == 0 && e->sectors[e->n].num_walls >= 0) ||
									(e->n && e->sectors[e->n].num_walls > 1))
		e->global_index--;
	if (e->mode == CLOSE)
		(e->mode)--;
	return (0);
}

static int		up_arrow_key_impl(t_main *m,
											t_map_editor *e, t_editor_sector *s)
{
	if (e->mode == TO && s->is_lift && s->to + 30 <= s->ceiling_height)
		s->to += 10;
	else if (e->mode == SPRITE_Z && e->sprite_cnt)
		e->sprites[e->sprite_cnt - 1].position.z += 1;
	else if (e->mode == SPRITE_TEXTURE &&
			e->sprites[e->sprite_cnt - 1].texture < TEXTURE_MAX)
		e->sprites[e->sprite_cnt - 1].texture += 1;
	else if (e->mode == DARKNESS && m->map.player.darkness < 10)
		m->map.player.darkness += 1;
	else if (e->mode == GRAVITY && m->map.player.gravity < 100)
		m->map.player.gravity += 1;
	else
		return (1);
	return (0);
}

int				up_arrow_key(t_main *m, t_map_editor *e)
{
	t_editor_sector *s;

	s = &e->sectors[e->n];
	if ((e->mode == TEXTURE || e->mode == CLOSE) && s->num_walls > 0 &&
							s->walls[s->num_walls - 1].texture < TEXTURE_MAX)
		s->walls[s->num_walls - 1].texture++;
	else if (e->mode == FLOOR_HEIGHT &&
									s->floor_height + 10 < s->ceiling_height)
		s->floor_height += 10;
	else if (e->mode == CEILING_HEIGHT &&
								s->ceiling_height + 10 <= MAX_CEILING_HEIGHT)
		s->ceiling_height += 10;
	else if (up_arrow_key_impl(m, e, s))
		return (1);
	return (0);
}

static int		down_arrow_key_impl(t_main *m,
									t_map_editor *e, t_editor_sector *s)
{
	if (e->mode == TO && s->is_lift)
		s->to -= 10;
	else if (e->mode == SPRITE_Z && e->sprite_cnt)
		e->sprites[e->sprite_cnt - 1].position.z -= 1;
	else if (e->mode == SPRITE_TEXTURE &&
					e->sprites[e->sprite_cnt - 1].texture > 0)
		e->sprites[e->sprite_cnt - 1].texture -= 1;
	else if (e->mode == DARKNESS && m->map.player.darkness > 0)
		m->map.player.darkness -= 1;
	else if (e->mode == GRAVITY && m->map.player.gravity > 3)
		m->map.player.gravity -= 1;
	else
		return (1);
	return (0);
}

int				down_arrow_key(t_main *m, t_map_editor *e)
{
	t_editor_sector *s;

	s = &e->sectors[e->n];
	if ((e->mode == TEXTURE || e->mode == CLOSE) &&
					s->num_walls > 0 && s->walls[s->num_walls - 1].texture > 0)
		s->walls[s->num_walls - 1].texture--;
	else if (e->mode == FLOOR_HEIGHT &&
									s->floor_height - 10 >= MIN_FLOOR_HEIGHT)
		s->floor_height -= 10;
	else if (e->mode == CEILING_HEIGHT &&
								s->ceiling_height - 10 > s->floor_height)
		s->ceiling_height -= 10;
	else if (down_arrow_key_impl(m, e, s))
		return (1);
	return (0);
}
