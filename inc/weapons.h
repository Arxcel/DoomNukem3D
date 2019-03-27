/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapons.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 21:51:08 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/27 17:01:33 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPONS_H
# define WEAPONS_H

#include "structure.h"
# define PSHIFT_X(i)	(((p.x - w->spos[i].x) + W) / 2)
# define PSHIFT_Y(i)	(H - (w->epos[i].y - p.y))

# define WEAPON_SCALE	2.35

bool			dn_init_weapons(t_wsys *wsys, t_tmap *t);

void			dn_handle_kdown_wchange_wstate(t_wsys *wsys, int key);
void			dn_handle_mbdown_wstate(t_wsys *wsys, SDL_MouseButtonEvent *b);
void			dn_handle_mbup_wstate(t_wsys *wsys, SDL_MouseButtonEvent *b);

void			dn_choose_weapon_render(t_wsys *wsys, t_tmap *t, t_img *i);

void			dn_render_pistol(t_weapon *w, t_tmap *t, t_state s, t_img *img);
void			pidle(t_weapon *w, t_tmap *t, t_img *i);
void			pshot(t_weapon *w, t_tmap *t, t_img *i);
void			preload(t_weapon *w, t_tmap *t, t_img *i);
void			pdraw(t_weapon *w, t_tmap *t, t_img *i);
void			phide(t_weapon *w, t_tmap *t, t_img *i);

# define _FPTR_ST typedef void (*fptr_st)(t_weapon*, t_tmap*, t_img*);

_FPTR_ST;

#endif
