/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 19:18:41 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/22 11:34:49 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
# define TEXTURES_H

# include "enum.h"
# include "structure.h"

# define WALL_SIZE	64
# define WALL_MAX_TEXTURES	8
# define TEXUTRES_MAP		"resources/textures_map.bmp"

# define WPNS_MAX_TEXTURES	57
# define WPNS_MAP		"assets/weapon/PC Computer - Doom Doom II - Weapons.png"
# define WPNS_TEX_BG	0xff00ffff
# define WPNS_MAP_BG	0xff95b1c8

bool	dn_init_textures_map(t_textures *tmap);
int		add_check_saved(point p,
		int already_saved_textures, point *spos, point *epos);

#endif
