/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_sector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 16:13:27 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 19:23:31 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static json_value		*pack_sector(json_value *vertices,
						json_value *neighbors, json_value *textures,
													t_editor_sector *s)
{
	json_value *sector;

	sector = json_object_new(8);
	json_object_push(sector, "floor_height",
							json_integer_new(s->floor_height));
	json_object_push(sector, "ceiling_height",
							json_integer_new(s->ceiling_height));
	json_object_push(sector, "vertices", vertices);
	json_object_push(sector, "neighbors", neighbors);
	json_object_push(sector, "textures", textures);
	json_object_push(sector, "is_lift", json_boolean_new(s->is_lift));
	json_object_push(sector, "from", json_integer_new(s->from));
	json_object_push(sector, "to", json_integer_new(s->to));
	return (sector);
}

static void				process_vert(t_map_editor *e, int i,
												int j, json_value **vert)
{
	json_value *obj;

	if (i > 0)
		j = 1;
	while (++j < e->sectors[i].num_walls)
	{
		obj = json_object_new(2);
		json_object_push(obj, "x",
			json_double_new(e->sectors[i].walls[j].begin.x / 10.0));
		json_object_push(obj, "y",
			json_double_new(e->sectors[i].walls[j].begin.y / 10.0));
		json_array_push(*vert, obj);
	}
}

static json_value		*process_sector(t_map_editor *e,
											int i, int j, json_value **vert)
{
	json_value *vertices;
	json_value *neighbors;
	json_value *textures;

	vertices = json_array_new(e->sectors[i].num_walls);
	neighbors = json_array_new(e->sectors[i].num_walls);
	textures = json_array_new(e->sectors[i].num_walls);
	process_vert(e, i, j, vert);
	j = -1;
	while (++j < e->sectors[i].num_walls)
	{
		json_array_push(vertices,
			json_integer_new(e->sectors[i].walls[j].global_index));
		json_array_push(neighbors,
			json_integer_new(e->sectors[i].neighbors[j]));
		json_array_push(textures,
			json_integer_new(e->sectors[i].walls[j].texture));
	}
	return (pack_sector(vertices, neighbors, textures, &e->sectors[i]));
}

json_value				*serialize_sectors(t_map_editor *e, json_value **vert)
{
	int			j;
	int			i;
	json_value	*arr_sect;

	j = -1;
	i = -1;
	arr_sect = json_array_new(e->n + 1);
	while (++i <= e->n)
		json_array_push(arr_sect, process_sector(e, i, j, vert));
	return (arr_sect);
}
