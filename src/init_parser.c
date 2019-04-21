/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:36:42 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/21 12:28:05 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_parser.h"
#include <sys/stat.h>

int						vertex_field(t_map *map, json_value *value)
{
	int			i;
	int			j;
	int			cnt;
	json_value	*js;

	if (count_vertices(map, value) || !(map->vertices =
	(t_vertex*)malloc(sizeof(t_vertex) * map->number_vertices)))
		return (1);
	i = 0;
	cnt = 0;
	while (i < value->u.array.length)
	{
		j = 0;
		js = value->u.array.values[i];
		while (j < js->u.object.values[1].value->u.array.length)
		{
			map->vertices[cnt].x =
				(float)js->u.object.values[1].value->u.array.values[j]->u.dbl;
			map->vertices[cnt].y = (float)js->u.object.values[0].value->u.dbl;
			j++;
			cnt++;
		}
		i++;
	}
	return (0);
}

int						analyze_value(t_map *map, json_object_entry js)
{
	if (!ft_strcmp(js.name, "vertex"))
	{
		if (map->vertices || vertex_field(map, js.value))
			return (1);
	}
	else if (!ft_strcmp(js.name, "sector"))
	{
		if (map->sectors || !map->vertices ||
			!map->number_vertices || !js.value ||
			js.value->type != json_array || sector_field(map, js.value))
			return (1);
	}
	else if (!ft_strcmp(js.name, "player"))
	{
		if (player_field(map, js.value))
			return (1);
	}
	else
		return (1);
	return (0);
}

int						parser_loop(t_map *map, json_value *value)
{
	int			i;

	if (value->type != json_object || value->u.object.length < 3)
		return (remove_values(map, value));
	i = 0;
	map->vertices = NULL;
	map->sectors = NULL;
	while (i < value->u.object.length)
	{
		if (analyze_value(map, value->u.object.values[i]))
			return (remove_values(map, value));
		i++;
	}
	return (0);
}

json_value				*init_json(char *file_name)
{
	char				*file_contents;
	json_value			*value;
	zip_file_t			*f;
	zip_t				*z;
	struct zip_stat		st;

	z = zip_open("./assets.res", 0, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_stat_init(&st);
	zip_stat(z, file_name, 0, &st);
	if (st.size < 1)
		MSG("No such map");
	file_contents = malloc(st.size);
	f = zip_fopen_encrypted(z, file_name, 0, "lol");
	if (!f)
		MSG(zip_strerror(z));
	if (zip_fread(f, file_contents, st.size) < 1 || zip_fclose(f))
		MSG(zip_strerror(z));
	if (!(value = json_parse(file_contents, st.size)))
		MSG("Unable to parse data");
	zip_close(z);
	free(file_contents);
	return (value);
}

int						parser(t_map *map, char *map_name)
{
	char			*name;
	json_value		*value;

	map->number_vertices = 0;
	map->number_sectors = 0;
	map->number_sprites = 0;
	map->number_enemies = 0;
	name = ft_strjoin("assets/maps/", map_name);
	if (!(value = init_json(name)) || parser_loop(map, value))
		MSG("Wrong data format");
	json_value_free(value);
	free(name);
	return (0);
}
