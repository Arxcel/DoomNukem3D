/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 21:51:08 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 20:24:44 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPONS_H
# define WEAPONS_H

# include "doom_nukem.h"

# define PSHIFT_X(i)	(((p.x - w->spos[i].x) + W) / 2)
# define PSHIFT_Y(i)	((H + w->epos[i].y) - w->spos[i].y)

# define WEAPON_SCALE_W	2
# define WEAPON_SCALE_H	WEAPON_SCALE_W

typedef enum	e_state
{
	idle,
	shot,
	reload,
	draw,
	hide,
	max_anim_states
} __attribute__((packed))				t_state;

typedef enum	e_current_weapon
{
	hands,
	pistol,
	sgun,
	ssgun,
	plasg,
	chsaw,
	rockl,
	cgun,
	bfg,
	max_weapons
} __attribute__((packed))				t_curr_w;

typedef struct	s_weapon
{
	int		fcount;
	point	*spos;
	point	*epos;
}				t_weapon;

typedef struct	s_weapons_system
{
	t_weapon	*hands;
	t_weapon	*pistol;
	t_weapon	*sgun;
	t_weapon	*ssgun;
	t_weapon	*plasg;
	t_weapon	*chsaw;
	t_weapon	*rockl;
	t_weapon	*cgun;
	t_weapon	*bfg;
	t_curr_w	wcurr;
	t_state		state;
	bool		no_action;
}				t_wsys;

bool			dn_init_weapons(t_wsys *wsys, t_tmap *t);

void			dn_handle_wchange_wstate(t_wsys *wsys, int key);

void			dn_choose_weapon_render(t_wsys *wsys, t_tmap *t, t_img *i);

void			dn_render_pistol(t_weapon *w, t_tmap *t, t_state s, t_img *img);
void			pidle(t_weapon *w, t_tmap *t, t_img *i);
void			pshot(t_weapon *w, t_tmap *t, t_img *i);
void			preload(t_weapon *w, t_tmap *t, t_img *i);
void			pdraw(t_weapon *w, t_tmap *t, t_img *i);
void			phide(t_weapon *w, t_tmap *t, t_img *i);

typedef void (*fn_ptr)(t_weapon*, t_tmap*, t_img*);

#endif
