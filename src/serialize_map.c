/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serialize_map.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/27 18:52:06 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/29 18:43:02 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

static json_value	*process_player(t_main *m)
{
	json_value	*pl;

	pl = json_object_new(6);
	json_object_push(pl, "x",
			json_double_new(m->map.player.position.x / 10.0));
	json_object_push(pl, "y",
			json_double_new(m->map.player.position.y / 10.0));
	json_object_push(pl, "angle", json_double_new(-90));
	json_object_push(pl, "sector",
			json_integer_new(m->map.player.sector_number));
	json_object_push(pl, "darkness", json_integer_new(m->map.player.darkness));
	json_object_push(pl, "gravity", json_double_new(m->map.player.gravity /
		10));
	return (pl);
}

static json_value	*process_sprites(t_map_editor *e)
{
	json_value	*arr_sprites;
	int			i;
	json_value	*sprite;

	i = -1;
	arr_sprites = json_array_new(e->sprite_cnt + 1);
	while (e->sprite_cnt && ++i < e->sprite_cnt)
	{
		sprite = json_object_new(4);
		json_object_push(sprite, "x",
				json_double_new(e->sprites[i].position.x));
		json_object_push(sprite, "y",
				json_double_new(e->sprites[i].position.y));
		json_object_push(sprite, "z",
				json_double_new(e->sprites[i].position.z));
		json_object_push(sprite, "texture",
				json_integer_new(e->sprites[i].texture));
		json_array_push(arr_sprites, sprite);
	}
	return (arr_sprites);
}

static json_value	*prepare_map(t_main *m, t_map_editor *e)
{
	json_value *obj;
	json_value *vert;
	json_value *arr_sect;
	json_value *arr_sprites;
	json_value *pl;

	obj = json_object_new(4);
	vert = json_array_new(WALLS_CNT * SECTORS_CNT);
	arr_sect = serialize_sectors(e, &vert);
	arr_sprites = process_sprites(e);
	pl = process_player(m);
	json_object_push(obj, "vertex", vert);
	json_object_push(obj, "sector", arr_sect);
	json_object_push(obj, "sprite", arr_sprites);
	json_object_push(obj, "player", pl);
	return (obj);
}

int					serialize_map(t_main *m, t_map_editor *e)
{
	json_value	*obj;
	char		*buf;

	obj = prepare_map(m, e);
	buf = malloc(json_measure(obj));
	json_serialize(buf, obj);
	json_builder_free(obj);
	write_map_to_file(buf, "assets/maps/map4.json");
	printf("%s\n", buf);
	free(buf);
	system("leaks doom-nukem");
	MSG("Map saved!");
	return (0);
}
