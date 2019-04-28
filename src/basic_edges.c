/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_edges.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:26:01 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/20 13:27:23 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		clamp_points(t_wall *w,
											t_vertex *i1, t_vertex *i2)
{
	if (w->t1.y < NEAR_Z)
		clamp_point(&w->t1, i1, i2);
	if (w->t2.y < NEAR_Z)
		clamp_point(&w->t2, i1, i2);
}

t_vertex		calculate_edges(t_player *player, t_vertex *vertex)
{
	t_vertex	res;

	res.x = (vertex->x - player->position.x) * player->anglesin -
						(vertex->y - player->position.y) * player->anglecos;
	res.y = (vertex->x - player->position.x) * player->anglecos +
						(vertex->y - player->position.y) * player->anglesin;
	return (res);
}

t_vertex		calculate_edges2(t_player *player, t_vertex *vertex)
{
	t_vertex	res;

	res.x = (vertex->x) * player->anglesin -
						(vertex->y) * player->anglecos;
	res.y = (vertex->x) * player->anglecos +
						(vertex->y) * player->anglesin;
	return (res);
}

void			clamp_edges_with_player_view(t_wall *w)
{
	t_vertex	i1;
	t_vertex	i2;
	t_vertex	org1;
	t_vertex	org2;

	if (w->t1.y > 0 && w->t2.y > 0)
		return ;
	i1 = intersect_line(w->t1, w->t2, (t_vertex){-NEAR_SIDE, NEAR_Z},
						(t_vertex){-FAR_SIDE, FAR_Z});
	i2 = intersect_line(w->t1, w->t2, (t_vertex){NEAR_SIDE, NEAR_Z},
						(t_vertex){FAR_SIDE, FAR_Z});
	org1 = w->t1;
	org2 = w->t2;
	clamp_points(w, &i1, &i2);
	if (fabsf(w->t2.x - w->t1.x) > fabsf(w->t2.y - w->t1.y))
	{
		w->u0 = (w->t1.x - org1.x) * 1023.0 / (org2.x - org1.x);
		w->u1 = (w->t2.x - org1.x) * 1023.0 / (org2.x - org1.x);
	}
	else
	{
		w->u0 = (w->t1.y - org1.y) * 1023.0 / (org2.y - org1.y);
		w->u1 = (w->t2.y - org1.y) * 1023.0 / (org2.y - org1.y);
	}
}
