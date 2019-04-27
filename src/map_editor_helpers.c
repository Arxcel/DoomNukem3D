#include "map_editor.h"

int					pnpoly(int num_walls, t_editor_wall *walls, t_dot dot)
{
	int c;

	c = 0;
	for (int i = 0, j = num_walls - 1; i < num_walls; j = i++) 
	{
		if ((
		(walls[i].begin.y < walls[j].begin.y) && (walls[i].begin.y <= dot.y)
		&& (dot.y <= walls[j].begin.y) &&
		((walls[j].begin.y - walls[i].begin.y) * (dot.x - walls[i].begin.x) >
		(walls[j].begin.x -  walls[i].begin.x) * (dot.y - walls[i].begin.y))
		) || (
		(walls[i].begin.y > walls[j].begin.y) &&
		(walls[j].begin.y <= dot.y) && (dot.y <= walls[i].begin.y) &&
		((walls[j].begin.y - walls[i].begin.y) * (dot.x - walls[i].begin.x) <
		(walls[j].begin.x - walls[i].begin.x) * (dot.y - walls[i].begin.y))
	))
		c = !c;
	}
	return c;
}

int		check_intersection(t_map_editor *e, t_dot mouse)
{
	int j;

	j = -1;
	while (++j < e->sectors[e->n].num_walls)
	{
		if (intersect(e->sectors[e->n].wall_vertice[j], mouse))
		{
			if (e->chosen == j)
			{
				e->chosen = -1;
				e->sectors[e->n].wall_vertice[j].color = YELLOW;
			}
			else if (e->chosen == -1)
			{
				e->chosen = j;
				e->sectors[e->n].wall_vertice[j].color = BLUE;
			}
			break;
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
	min = e->sectors[0].wall_vertice[0].begin;
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			if (e->sectors[i].wall_vertice[j].begin.x < min.x)
				min.x = e->sectors[i].wall_vertice[j].begin.x;
			if (e->sectors[i].wall_vertice[j].begin.y < min.y)
				min.y = e->sectors[i].wall_vertice[j].begin.y;
		}
	i = -1;
	while (++i <= e->n && (j = -1))
		while (++j < e->sectors[i].num_walls)
		{
			e->sectors[i].wall_vertice[j].begin.x -= min.x;
			e->sectors[i].wall_vertice[j].begin.y -= min.y;
			e->sectors[i].wall_vertice[j].end.x -= min.x;
			e->sectors[i].wall_vertice[j].end.y -= min.y;
		}
}
