/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_edges.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 14:26:01 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/13 14:38:05 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static void		clamp_points(t_renderer *r, t_wall *w,
											t_vertex *i1, t_vertex *i2)
{
	if (r->t1.y < NEAR_Z)
		clamp_point(&r->t1, i1, i2);
	if (r->t2.y < NEAR_Z)
		clamp_point(&r->t2, i1, i2);
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

void			clamp_edges_with_player_view(t_renderer *r, t_wall *w)
{
	t_vertex	i1;
	t_vertex	i2;
	t_vertex	org1;
	t_vertex	org2;

	if (r->t1.y > 0 && r->t2.y > 0)
		return ;
	i1 = intersect_line(r->t1, r->t2, (t_vertex){-NEAR_SIDE, NEAR_Z},
						(t_vertex){-FAR_SIDE, FAR_Z});
	i2 = intersect_line(r->t1, r->t2, (t_vertex){NEAR_SIDE, NEAR_Z},
						(t_vertex){FAR_SIDE, FAR_Z});
	org1 = r->t1;
	org2 = r->t2;
	clamp_points(r, w, &i1, &i2);
	if (fabsf(r->t2.x - r->t1.x) > fabsf(r->t2.y - r->t1.y))
	{
		w->u0 = (r->t1.x - org1.x) * 1023.0 / (org2.x - org1.x);
		w->u1 = (r->t2.x - org1.x) * 1023.0 / (org2.x - org1.x);
	}
	else
	{
		w->u0 = (r->t1.y - org1.y) * 1023.0 / (org2.y - org1.y);
		w->u1 = (r->t2.y - org1.y) * 1023.0 / (org2.y - org1.y);
	}
}
