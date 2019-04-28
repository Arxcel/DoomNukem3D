/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lift.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/28 19:11:54 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/28 19:39:42 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void	process_lift(t_main *m, t_sector *lift)
{
	float tmp;

	if (lift->is_activated)
	{
		lift->floor_height += (lift->to - lift->from) * m->delta_time * LIFT_SPEED / 10.0;
		if (fabsf(lift->floor_height - lift->to) < 3)
		{
			tmp = lift->to;
			lift->to = lift->from;
			lift->from = tmp;
			lift->is_activated = false;
		}
	}
}

void		calc_lifts(t_main *m)
{
	int i;

	i = -1;
	while (++i < m->map.number_sectors)
	{
		if (!m->map.sectors[i].is_lift || !m->map.sectors[i].is_activated)
			continue ;
		process_lift(m, &m->map.sectors[i]);
	}
}