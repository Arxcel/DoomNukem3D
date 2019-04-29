/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_editor_lines.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:48:55 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 17:52:59 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

t_dot	module(t_dot a, t_dot b)
{
	t_dot d;

	d.x = abs(b.x - a.x);
	d.y = abs(b.y - a.y);
	return (d);
}

t_dot	direction(t_dot a, t_dot b)
{
	t_dot s;

	s.x = a.x < b.x ? 1 : -1;
	s.y = a.y < b.y ? 1 : -1;
	return (s);
}

bool	compare(t_dot a, t_dot b)
{
	return ((a.x == b.x - 1 && a.y == b.y - 1)
		|| (a.x == b.x + 1 && a.y == b.y + 1)
		|| (a.x == b.x && a.y == b.y) || (a.x == b.x - 1 && a.y == b.y + 1)
		|| (a.x == b.x + 1 && a.y == b.y - 1));
}

int		intersect(t_editor_wall wall, t_dot cur)
{
	t_dot error;
	t_dot ds[2];

	ds[0] = module(wall.begin, wall.end);
	ds[1] = direction(wall.begin, wall.end);
	error.x = (ds[0].x > ds[0].y ? ds[0].x : -ds[0].y) / 2;
	while (1)
	{
		if (compare(wall.begin, cur))
			return (1);
		if (wall.begin.x == wall.end.x && wall.begin.y == wall.end.y)
			break ;
		error.y = error.x;
		if (error.y > -ds[0].x)
		{
			error.x -= ds[0].y;
			wall.begin.x += ds[1].x;
		}
		if (error.y < ds[0].y)
		{
			error.x += ds[0].x;
			wall.begin.y += ds[1].y;
		}
	}
	return (0);
}

int		line(t_editor_wall wall, t_main *main)
{
	t_dot error;
	t_dot d;
	t_dot s;

	d = module(wall.begin, wall.end);
	s = direction(wall.begin, wall.end);
	error.x = (d.x > d.y ? d.x : -d.y) / 2;
	while (1)
	{
		print_vector(wall, main);
		if (wall.begin.x == wall.end.x && wall.begin.y == wall.end.y)
			break ;
		error.y = error.x;
		if (error.y > -d.x)
		{
			error.x -= d.y;
			wall.begin.x += s.x;
		}
		if (error.y < d.y)
		{
			error.x += d.x;
			wall.begin.y += s.y;
		}
	}
	return (0);
}
