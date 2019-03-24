/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 19:18:41 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/25 00:20:28 by tmaluh           ###   ########.fr       */
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

# ifdef __APPLE__
#  define WPNS_TEX_BG	0xff00ffff
#  define WPNS_MAP_BG	0xb195c8b1
# endif

# ifdef __linux__
#  define WPNS_TEX_BG	0xffff
#  define WPNS_MAP_BG	0x95b1c8
# endif

# define PISTOL_ICO			2
# define PISTOL_SHOT_EFF	0
# define PISTOL_AFTER_SHOT4	1
# define PISTOL_AFTER_SHOT3	8
# define PISTOL_AFTER_SHOT2	7
# define PISTOL_AFTER_SHOT1	6
# define PISTOL				10

# define CHAINSAW_ICO	5
# define CHAINSAW1		3
# define CHAINSAW2		4
# define CHAINSAW3		11
# define CHAINSAW4		12

# define HANDS_ATTACK2	9
# define HANDS_ATTACK1	13
# define HANDS_RIGHT	14
# define HANDS_LEFT		15

# define SHOTGUN_ICO		28
# define SHOTGUN_SHOT_EFF2	16
# define SHOTGUN_SHOT_EFF1	17
# define SHOTGUN_RELOAD1	23
# define SHOTGUN_RELOAD2	19
# define SHOTGUN_RELOAD3	21
# define SHOTGUN			31

# define SUPER_SHOTGUN_ICO			29
# define SUPER_SHOTGUN_SHOT_EFF1	18
# define SUPER_SHOTGUN_SHOT_EFF2	20
# define SUPER_SHOTGUN_RELOAD1		30
# define SUPER_SHOTGUN_RELOAD2		33
# define SUPER_SHOTGUN_RELOAD3		26
# define SUPER_SHOTGUN_RELOAD4		27
# define SUPER_SHOTGUN_RELOAD5		25
# define SUPER_SHOTGUN_RELOAD6		24
# define SUPER_SHOTGUN_RELOAD7		22
# define SUPER_SHOTGUN				32

# define CGUN_ICO		36
# define CGUN_SHOT_EFF2	38
# define CGUN_SHOT_EFF1	40
# define CGUN_SHOT2		49
# define CGUN_SHOT1		47

# define ROCKETL_ICO		53
# define ROCKETL_SHOT_EFF4	34
# define ROCKETL_SHOT_EFF3	37
# define ROCKETL_SHOT_EFF2	39
# define ROCKETL_SHOT_EFF1	43
# define ROCKETL_SHOT		52
# define ROCKETL			51

# define BFG_ICO		42
# define BFG_SHOT_EFF2	35
# define BFG_SHOT_EFF1	41
# define BFG_SHOT2		46
# define BFG_SHOT1		48
# define BFG			45

# define PLASMAG_ICO	50
# define PLASMAG_RELOAD	44
# define PLASMAG_SHOT1	54
# define PLASMAG_SHOT2	55
# define PLASMAG		56

bool	dn_init_textures_map(t_textures *tmap);
int		dn_check_saved_texture(point p, int already_saved_textures,
								point *spos, point *epos);
bool	dn_init_ck_map(t_hinit h);

#endif
