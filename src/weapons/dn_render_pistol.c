/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_render_pistol.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 12:05:09 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 13:52:14 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	dn_render_pistol(t_weapon *w, t_tmap *t, t_state s, t_img *img)
{
	const fn_ptr	fn_states[] = {pidle, pshot, preload, pdraw, phide};
	int				i;

	i = -1;
	while (++i < max_anim_states)
		if (s == i)
			fn_states[i](w, t, img);
}
