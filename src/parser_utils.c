/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:46:44 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/13 12:46:47 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_parser.h"

int						count_vertices(t_map *map, json_value *value)
{
	int					i;
	json_value			*js;

	if (value->type != json_array)
		return (1);
	i = 0;
	while (i < value->u.array.length)
	{
		js = value->u.array.values[i];
		if (js->type != json_object ||
			ft_strcmp(js->u.object.values[0].name, "y") ||
			ft_strcmp(js->u.object.values[1].name, "x") ||
			js->u.object.values[1].value->type != json_array)
			return (1);
		js = js->u.object.values[1].value;
		map->number_vertices += js->u.array.length;
		i++;
	}
	if (!map->number_vertices)
		return (1);
	return (0);
}

int						find_min_max(t_map *map, int i, json_value *js)
{
	t_sector			*sect;
	int					num;
	t_vertex			t;

	sect = &map->sectors[i];
	num = sect->number_vertices;
	i = -1;
	sect->min = map->vertices[js->u.array.values[0]->u.integer];
	sect->max = map->vertices[js->u.array.values[0]->u.integer];
	while (++i < num)
	{
		t = map->vertices[js->u.array.values[i]->u.integer];
		sect->vertices[i + 1] = t;
		if (map->vertices[js->u.array.values[i]->u.integer].x >= sect->max.x)
			sect->max.x = map->vertices[js->u.array.values[i]->u.integer].x;
		if (map->vertices[js->u.array.values[i]->u.integer].y >= sect->max.y)
			sect->max.y = map->vertices[js->u.array.values[i]->u.integer].y;
		if (map->vertices[js->u.array.values[i]->u.integer].x <= sect->min.x)
			sect->min.x = map->vertices[js->u.array.values[i]->u.integer].x;
		if (map->vertices[js->u.array.values[i]->u.integer].y <= sect->min.y)
			sect->min.y = map->vertices[js->u.array.values[i]->u.integer].y;
	}
	sect->vertices[0] = sect->vertices[num];
	return (0);
}

int						check_obj(json_object_entry *obj, t_sector *sect)
{
	int	j;

	if (obj[2].value->type != json_array ||
		obj[3].value->type != json_array ||
		obj[4].value->type != json_array ||
		obj[2].value->u.array.length != obj[3].value->u.array.length ||
		obj[2].value->u.array.length != obj[4].value->u.array.length)
		return (1);
	sect->floor_height = obj[0].value->u.integer;
	sect->ceil_height = obj[1].value->u.integer;
	sect->number_vertices = obj[2].value->u.array.length;
	if (!(sect->neighbors = malloc(sect->number_vertices * sizeof(short))) ||
		!(sect->vertices = malloc((sect->number_vertices + 1) *
		sizeof(t_vertex))) ||
		!(sect->textures = (int*)malloc(sect->number_vertices * sizeof(int))))
		return (1);
	j = -1;
	while (++j < sect->number_vertices)
	{
		sect->neighbors[j] = (short)obj[3].value->u.array.values[j]->u.integer;
		sect->textures[j] = obj[4].value->u.array.values[j]->u.integer;
	}
	return (0);
}

int						sector_field(t_map *map, json_value *value)
{
	int					i;
	json_object_entry	*obj;

	map->number_sectors = value->u.array.length;
	if (!(map->sectors =
		(t_sector*)malloc(sizeof(t_sector) * map->number_sectors)))
		return (1);
	i = -1;
	while (++i < map->number_sectors)
	{
		map->sectors[i].vertices = NULL;
		map->sectors[i].neighbors = NULL;
		map->sectors[i].textures = NULL;
		obj = value->u.array.values[i]->u.object.values;
		if (check_obj(obj, &map->sectors[i]))
		{
			map->number_sectors = i;
			return (1);
		}
		find_min_max(map, i, obj[2].value);
	}
	return (0);
}

int						player_field(t_map *map, json_value *value)
{
	json_object_entry	*js;

	if (!value || value->type != json_object || value->u.object.length < 4)
		return (1);
	js = value->u.object.values;
	if (ft_strcmp(js[0].name, "x") || ft_strcmp(js[1].name, "y") ||
		ft_strcmp(js[2].name, "angle") || ft_strcmp(js[3].name, "sector"))
		return (1);
	map->player.angle = (float)js[2].value->u.dbl;
	map->player.sector_number = js[3].value->u.integer;
	map->player.velocity.x = 0;
	map->player.velocity.y = 0;
	map->player.velocity.z = 0;
	map->player.anglesin = 0;
	map->player.anglecos = 0;
	map->player.pitch = 0;
	map->player.position.x = (float)js[0].value->u.dbl;
	map->player.position.y = (float)js[1].value->u.dbl;
	map->player.position.z =
		map->sectors[map->player.sector_number].floor_height + STANDHEIGHT;
	return (0);
}
