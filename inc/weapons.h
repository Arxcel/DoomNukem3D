/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 21:51:08 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/25 23:48:15 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPONS_H
# define WEAPONS_H

# include "textures.h"

typedef enum	e_state
{
	idle,
	shot,
	reload,
	draw,
	hide
} __attribute__((packed))				t_state;

typedef struct	s_weapon
{
	t_state	state;
	int		fcount;
	point	*spos;
	point	*epos;
}				t_weapon;


typedef struct	s_weapons_system
{
	t_weapon	*pistol;
	t_weapon	*hands;
	t_weapon	*plasg;
	t_weapon	*chsaw;
	t_weapon	*rockl;
	t_weapon	*ssgun;
	t_weapon	*sgun;
	t_weapon	*cgun;
	t_weapon	*bfg;
}				t_wsys;

bool			dn_init_weapons(t_wsys *wsys, t_tmap *t);

#endif
