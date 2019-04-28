/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 15:31:11 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/27 15:38:21 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

void	calc_green_time(t_main *m)
{
	if (m->greenify)
	{
		m->pseudo_time += (m->curr_time - m->prev_time) /
								(float)(SDL_GetPerformanceFrequency());
		if (m->pseudo_time > 0.5)
		{
			m->pseudo_time = 0;
			m->greenify = false;
		}
	}
}
