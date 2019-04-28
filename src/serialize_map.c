/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:52:06 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/28 12:42:14 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json-builder.h"
#include "doom_nukem.h"
#include <unistd.h>

int    write_map_to_file(char *buf, const char *filename)
{
	zip_t				*z;
	zip_source_t		*s;

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));

	if ((s = zip_source_buffer(z, buf, sizeof(buf), 0)) == NULL ||
		zip_file_add(z, filename, s, ZIP_FL_OVERWRITE) < 0)
	{
		zip_source_free(s);
		printf("error adding file: %s\n", zip_strerror(z));
	}
	zip_close(z);
	printf("File saved\n");
	return 0;
}

int     serialize_map(t_main *m, t_editor_sector *sectors, int num_sect)
{
    json_value *obj = json_object_new(3);
    json_value *vert = json_array_new(WALLS_CNT * SECTORS_CNT);
    json_value *arr_sect = json_array_new(num_sect + 1);

    int i = -1;
    int j = -1;
    while (++i <= num_sect)
    {
        
        json_value *sector = json_object_new(5);

        json_value *vertices = json_array_new(sectors[i].num_walls);
        json_value *neighbors = json_array_new(sectors[i].num_walls);
        json_value *textures = json_array_new(sectors[i].num_walls);
        if (i > 0)
            j = 1;
        while (++j < sectors[i].num_walls)
        {
            json_value *_obj = json_object_new(2);
            json_object_push(_obj, "x", json_double_new(sectors[i].wall_vertice[j].begin.x));
            json_object_push(_obj, "y", json_double_new(sectors[i].wall_vertice[j].begin.y));
            json_array_push(vert, _obj);
        }
        int j = -1;
        while (++j < sectors[i].num_walls)
        {
            json_array_push(vertices, json_integer_new(sectors[i].wall_vertice[j].global_index));
            json_array_push(neighbors, json_integer_new(sectors[i].neighbors[j]));
            json_array_push(textures, json_integer_new(sectors[i].wall_vertice[j].texture));

        }
        json_object_push(sector, "floor_height", json_integer_new(sectors[i].floor_height));
        json_object_push(sector, "ceiling_height", json_integer_new(sectors[i].ceiling_height));
        json_object_push(sector, "vertices", vertices);
        json_object_push(sector, "neighbors", neighbors);
        json_object_push(sector, "textures", textures);
        json_array_push(arr_sect, sector);
    }
    json_value *pl = json_object_new(4);
    json_object_push(pl, "x", json_double_new(m->map.player.position.x));
    json_object_push(pl, "y", json_double_new(m->map.player.position.y));
    json_object_push(pl, "angle", json_double_new(-90));
    json_object_push(pl, "sector", json_integer_new(m->map.player.sector_number));
    json_object_push(obj, "vertex", vert);
    json_object_push(obj, "sector", arr_sect);
    json_object_push(obj, "player", pl);
    char *buf = malloc(json_measure(obj));
    json_serialize(buf, obj);
    json_value_free(obj);
    write_map_to_file(buf, "assets/maps/map5.json");
    free(buf);
    return (0);
}
