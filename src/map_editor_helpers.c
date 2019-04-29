/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:25 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 19:23:31 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		pnpoly(int num_walls, t_editor_wall *w, t_dot d)
{
	int c;
	int i;
	int j;

	c = 0;
	i = 0;
	j = num_walls - 1;
	while (i < num_walls)
	{
		if ((
			(w[i].begin.y < w[j].begin.y) && (w[i].begin.y <= d.y)
			&& (d.y <= w[j].begin.y) && ((w[j].begin.y - w[i].begin.y)
			* (d.x - w[i].begin.x) > (w[j].begin.x - w[i].begin.x)
			* (d.y - w[i].begin.y))) || ((w[i].begin.y > w[j].begin.y)
			&& (w[j].begin.y <= d.y) && (d.y <= w[i].begin.y) &&
			((w[j].begin.y - w[i].begin.y) * (d.x - w[i].begin.x) <
			(w[j].begin.x - w[i].begin.x) * (d.y - w[i].begin.y))))
			c = !c;
		j = i++;
	}
	return (c);
}

int		check_intersection(t_map_editor *e, t_dot mouse)
{
	int j;

	j = -1;
	while (++j < e->sectors[e->n].num_walls)
	{
		if (intersect(e->sectors[e->n].walls[j], mouse))
		{
			if (e->chosen == j)
			{
				e->chosen = -1;
				e->sectors[e->n].walls[j].color = YELLOW;
			}
			else if (e->chosen == -1)
			{
				e->chosen = j;
				e->sectors[e->n].walls[j].color = BLUE;
			}
			break ;
		}
	}
	return (e->chosen);
}

void	shift_left(t_map_editor *e)
{
	t_dot	min;
	int		i;
	int		j;
	int		gl;

	gl = 0;
	min = e->sectors[0].walls[0].begin;
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			if (e->sectors[i].walls[j].begin.x < min.x)
				min.x = e->sectors[i].walls[j].begin.x;
			if (e->sectors[i].walls[j].begin.y < min.y)
				min.y = e->sectors[i].walls[j].begin.y;
		}
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			e->sectors[i].walls[j].begin.x -= min.x;
			e->sectors[i].walls[j].begin.y -= min.y;
			e->sectors[i].walls[j].end.x -= min.x;
			e->sectors[i].walls[j].end.y -= min.y;
		}
}
