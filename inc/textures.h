/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 19:18:41 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 18:37:15 by tmaluh           ###   ########.fr       */
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
#  define WPNS_MAP_BG	0xff95b1c8
#  define IS_FORMAT_SURF	0
# endif

# ifdef __linux__
#  define WPNS_TEX_BG	0xffff
#  define WPNS_MAP_BG	0x95b1c8
#  define IS_FORMAT_SURF	1
# endif

/*
**	All short named defines used only for above fucking norme.
**
**	- Someting about this defines:
**		H	- Hands
**		P	- Pistol
**		S	- Shotgun
**		SS	- Super Shotgun
**		CS	- Chain Saw
**		CG	- Chain Gun
**		PG	- Plasma Gun
**		RL	- Rocket Launcher
**		BFG - Big Fucking Gun
**
**	- Spaecifiers:
**		S		- Shot
**		R		- Reload
**		SHE		- Shot Effect
**		FRMS	- Summary frames counter
**		AS		- After shot
**
**		\ Only for H(hands):
**			ATK	- Attack
**			R	- Right
**			L	- Left
**
**	EXAMPLE:
**		PSHE - Pistol Shot Effect
*/
# define P_ICO	2
# define PFRMS	6
# define PSHE	0
# define PAS4	1
# define PAS3	8
# define PAS2	7
# define PAS1	6
# define PISTOL	10

# define CS_ICO	5
# define CSFRMS	4
# define CS1	3
# define CS2	4
# define CS3	11
# define CS4	12

# define HFRMS	4
# define HATK2	9
# define HATK1	13
# define HR	14
# define HL	15

# define S_ICO	28
# define SFRMS	6
# define SSHE2	16
# define SSHE1	17
# define SR1	23
# define SR2	19
# define SR3	21
# define S		31

# define SS_ICO	29
# define SSFRMS	10
# define SSSE1	18
# define SSSE2	20
# define SSR1	30
# define SSR2	33
# define SSR3	26
# define SSR4	27
# define SSR5	25
# define SSR6	24
# define SSR7	22
# define SS		32

# define CG_ICO	36
# define CGFRMS	4
# define CGSHE2	38
# define CGSHE1	40
# define CGS2	49
# define CGS1	47

# define RL_ICO	53
# define RLFRMS	6
# define RLSE4	34
# define RLSE3	37
# define RLSE2	39
# define RLSE1	43
# define RLS	52
# define RL		51

# define BFG_ICO	42
# define BFGFRMS	5
# define BFGSHE2	35
# define BFGSHE1	41
# define BFGS2		46
# define BFGS1		48
# define BFG		45

# define PGICO	50
# define PGFRMS	4
# define PGR	44
# define PGS1	54
# define PGS2	55
# define PG		56

typedef struct	s_textures_map
{
	SDL_Surface	*surf;
	Uint32		*pxls;
	point		s;
	point		*spos;
	point		*epos;
	int			tmax;
}				t_tmap;

typedef struct		s_textures
{
	t_tmap	wpns;
}					t_textures;

typedef struct	s_init_helper
{
	t_tmap	*t;
	char	*path;
	Uint32	ck_color;
	Uint32	bg_color;
	int		max_textures;
}				t_hinit;

typedef struct	s_copy_helper
{
	point	*ssrc;
	point	*esrc;
	point	*sdst;
	point	*edst;
	int		max;
}				t_hcp;

bool			dn_init_textures_map(t_textures *tmap);
int				dn_check_saved_texture(point p, int already_saved_textures,
										point *spos, point *epos);
bool			dn_init_ck_map(t_hinit h, point scale);

#endif
