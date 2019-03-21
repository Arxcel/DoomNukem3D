/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 19:18:41 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/21 19:31:44 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURES_H
# define TEXTURES_H

# include "enum.h"
# include "structure.h"

# define WALL_SIZE	64
# define WALL_MAX_TEXTURES	8
# define TEXUTRES_MAP "resources/textures_map.bmp"
# define WEAPONS_MAP "assets/weapon/PC Computer - Doom Doom II - Weapons.png"
# define WPNS_TEX_BG    0x00ffff
# define WPNS_MAP_BG    0x95b1c8

bool				dn_init_textures_map(t_textures *tmap);

#endif
