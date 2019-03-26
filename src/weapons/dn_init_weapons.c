/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dn_init_weapons.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 22:30:30 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/26 12:50:45 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static bool	acp_frames(t_weapon **w, t_tmap *t, int frames[], int fcount)
{
	int	i = -1;

	_ISZ(t_weapon, *w, 1);
	(*w)->fcount = fcount;
	_ISZ(point, (*w)->spos, (*w)->fcount);
	_ISZ(point, (*w)->epos, (*w)->fcount);
	while (++i < (*w)->fcount)
	{
		(*w)->spos[i] = t->spos[frames[i]];
		(*w)->epos[i] = t->epos[frames[i]];
	}
	return true;
}

bool	dn_init_weapons(t_wsys *wsys, t_tmap *t)
{
	_NOTIS_F(acp_frames(&wsys->pistol, t,
		(int[]){PISTOL, PSHE, PAS1, PAS2, PAS3, PAS4}, PFRMS));
	_NOTIS_F(acp_frames(&wsys->hands, t, (int[]){HR, HL, HATK1, HATK2}, HFRMS));
	_NOTIS_F(acp_frames(&wsys->plasg, t, (int[]){PG, PGS1, PGS2, PGR}, PGFRMS));
	_NOTIS_F(acp_frames(&wsys->chsaw, t, (int[]){CS1, CS2, CS3, CS4}, CSFRMS));
	_NOTIS_F(acp_frames(&wsys->rockl, t,
		(int[]){RL, RLS, RLSE1, RLSE2, RLSE3, RLSE4}, RLFRMS));
	_NOTIS_F(acp_frames(&wsys->ssgun, t, (int[]){SS, SSSE1, SSSE2, SSR1, SSR2,
									SSR3, SSR4, SSR5, SSR6, SSR7}, SSFRMS));
	_NOTIS_F(acp_frames(&wsys->sgun, t,
		(int[]){S, SSHE1, SSHE2, SR1, SR2, SR3}, SFRMS));
	_NOTIS_F(acp_frames(&wsys->cgun, t,
		(int[]){CGS1, CGS2, CGSHE1, CGSHE2}, CGFRMS));
	_NOTIS_F(acp_frames(&wsys->bfg, t,
		(int[]){BFG, BFGS1, BFGS2, BFGSHE1, BFGSHE2}, BFGFRMS));
	wsys->state = idle;
	wsys->wcurr = pistol;
	wsys->no_action = true;
	return (true);
}
