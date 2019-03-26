/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_choose_current_weapon_render.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 12:36:26 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 13:01:53 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	dn_choose_current_weapon_render(t_wsys *wsys, t_tmap *t)
{
	if (wsys->wcurr == hands)
		printf("hands\n");
	else if (wsys->wcurr == pistol)
		dn_render_pistol(wsys->pistol, t, wsys->state);
	else if (wsys->wcurr == sgun)
		printf("shotgun\n");
	else if (wsys->wcurr == ssgun)
		printf("super shotgun\n");
	else if (wsys->wcurr == plasg)
		printf("plasma gun\n");
	else if (wsys->wcurr == chsaw)
		printf("chainsaw\n");
	else if (wsys->wcurr == rockl)
		printf("rocket launcer\n");
	else if (wsys->wcurr == cgun)
		printf("chaingun\n");
	else if (wsys->wcurr == bfg)
		printf("bfg\n");
}
