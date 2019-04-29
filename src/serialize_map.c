/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:52:06 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 14:54:13 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "json-builder.h"
#include "doom_nukem.h"
#include <unistd.h>

int    write_map_to_file(char *buf, const char *filename)
{
	zip_t				*z;
	zip_source_t		*s;
	zip_int64_t			index;
	int					res;
	zip_error_t			e;

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_set_default_password(z, RESOURCES_PASS);
	s = zip_source_buffer_create(buf, ft_strlen(buf), 0, &e);
	if (!s)
		MSG(e.str);
	index = zip_name_locate(z, filename, 0);
	if (index < 0)
		MSG(zip_strerror(z));
	zip_delete(z, index);
	res = zip_file_add(z, filename, s, ZIP_FL_ENC_UTF_8);
	if (res < 0)
	{
		zip_source_free(s);
		MSG(zip_strerror(z));
	}
	zip_close(z);
	return 0;
}

int     serialize_map(t_main *m, t_map_editor *e)
{
    json_value *obj = json_object_new(3);
    json_value *vert = json_array_new(WALLS_CNT * SECTORS_CNT);
    json_value *arr_sect = json_array_new(e->n + 1);

    int i = -1;
    int j = -1;
    while (++i <= e->n)
    {
        
        json_value *sector = json_object_new(8);

        json_value *vertices = json_array_new(e->sectors[i].num_walls);
        json_value *neighbors = json_array_new(e->sectors[i].num_walls);
        json_value *textures = json_array_new(e->sectors[i].num_walls);
        if (i > 0)
            j = 1;
        while (++j < e->sectors[i].num_walls)
        {
            json_value *_obj = json_object_new(2);
            json_object_push(_obj, "x", json_double_new(e->sectors[i].wall_vertice[j].begin.x / 10.0));
            json_object_push(_obj, "y", json_double_new(e->sectors[i].wall_vertice[j].begin.y / 10.0));
            json_array_push(vert, _obj);
        }
        int j = -1;
        while (++j < e->sectors[i].num_walls)
        {
            json_array_push(vertices, json_integer_new(e->sectors[i].wall_vertice[j].global_index));
            json_array_push(neighbors, json_integer_new(e->sectors[i].neighbors[j]));
            json_array_push(textures, json_integer_new(e->sectors[i].wall_vertice[j].texture));

        }
        json_object_push(sector, "floor_height", json_integer_new(e->sectors[i].floor_height));
        json_object_push(sector, "ceiling_height", json_integer_new(e->sectors[i].ceiling_height));
        json_object_push(sector, "vertices", vertices);
        json_object_push(sector, "neighbors", neighbors);
        json_object_push(sector, "textures", textures);
        json_object_push(sector, "is_lift", json_boolean_new(e->sectors[i].is_lift));
        json_object_push(sector, "from", json_integer_new(e->sectors[i].from));
        json_object_push(sector, "to", json_integer_new(e->sectors[i].to));
        json_array_push(arr_sect, sector);
    }
    json_value *arr_sprites = json_array_new(e->sprite_cnt + 1);
    i = -1;
    while(e->sprite_cnt && ++i <= e->sprite_cnt)
    {
        json_value *sprite = json_object_new(4);
        json_object_push(sprite, "x", json_double_new(e->sprites[i].position.x));
        json_object_push(sprite, "y", json_double_new(e->sprites[i].position.y));
        json_object_push(sprite, "z", json_double_new(e->sprites[i].position.z));
        json_object_push(sprite, "texture", json_integer_new(e->sprites[i].texture));
        json_array_push(arr_sprites, sprite);
    }
    
    json_value *pl = json_object_new(6);
    json_object_push(pl, "x", json_double_new(m->map.player.position.x / 10.0));
    json_object_push(pl, "y", json_double_new(m->map.player.position.y / 10.0));
    json_object_push(pl, "angle", json_double_new(-90));
    json_object_push(pl, "sector", json_integer_new(m->map.player.sector_number));
    json_object_push(pl, "darkness", json_integer_new(m->map.player.darkness));
    json_object_push(pl, "gravity", json_double_new(m->map.player.gravity));



    json_object_push(obj, "vertex", vert);
    json_object_push(obj, "sector", arr_sect);
    json_object_push(obj, "sprite", arr_sprites);
    json_object_push(obj, "player", pl);
    char *buf = malloc(json_measure(obj));
    json_serialize(buf, obj);
    json_value_free(obj);
    write_map_to_file(buf, "assets/maps/map4.json");
    puts(buf);
    free(buf);
	MSG("Map saved!");
    return (0);
}
