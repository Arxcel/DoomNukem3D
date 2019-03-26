/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_sdl_handle_key_wchoose_wstate.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 12:48:11 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 13:43:49 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	add_change_weapons_handler(t_wsys *wsys, int key)
{
	if (key == SDLK_1)
		wsys->wcurr = hands;
	else if (key == SDLK_2)
		wsys->wcurr = pistol;
	else if (key == SDLK_3)
		wsys->wcurr = sgun;
	else if (key == SDLK_4)
		wsys->wcurr = ssgun;
	else if (key == SDLK_5)
		wsys->wcurr = plasg;
	else if (key == SDLK_6)
		wsys->wcurr = chsaw;
	else if (key == SDLK_7)
		wsys->wcurr = rockl;
	else if (key == SDLK_8)
		wsys->wcurr = cgun;
	else if (key == SDLK_9)
		wsys->wcurr = bfg;
}

void	dn_handle_wchange_wstate(t_wsys *wsys, int key)
{
	if (wsys->no_action)
	{
		if (key == SDLK_r)
			wsys->state = reload;
		else
			add_change_weapons_handler(wsys, key);
	}
}
