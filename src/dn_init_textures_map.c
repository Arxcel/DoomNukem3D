/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_textures_map.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:46:33 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/18 17:53:31 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

bool	dn_init_textures_map(t_tex_map *tmap)
{
	t_point	p;
	int		i;

	i = -1;
	p = (t_point){0, 0};
	_NOTIS_F(tmap->surface = sdl_load_surface(TEXUTRES_MAP));
	while (++i < WALL_MAX_TEXTURES)
	{
		tmap->walls_pos[i] = (t_point){p.x, p.y};
		p.x += WALLS_BLOCK_SIZE;
		if (i + 1 == WALL_MAX_TEXTURES / 2)
			p = (t_point){0, p.y + WALLS_BLOCK_SIZE};
	}
	return (true);
}
