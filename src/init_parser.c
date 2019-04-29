/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:36:42 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/28 18:12:53 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"
#include <sys/stat.h>

int						vertex_field(t_map *map, json_value *value)
{
	size_t		cnt;
	json_value	*js;

	if (value->type != json_array || value->u.array.length < 3 ||
	!(map->vertices = (t_vertex*)malloc(sizeof(t_vertex) *
			value->u.array.length)))
		return (1);
	map->number_vertices = value->u.array.length;
	cnt = -1;
	while (++cnt < value->u.array.length)
	{
		js = value->u.array.values[cnt];
		if (js->type != json_object
			|| ft_strcmp(js->u.object.values[0].name, "x")
			|| ft_strcmp(js->u.object.values[1].name, "y"))
			return (1);
		map->vertices[cnt].x = (float)js->u.object.values[0].value->u.dbl;
		map->vertices[cnt].y = (float)js->u.object.values[1].value->u.dbl;
	}
	return (0);
}

int						analyze_value(t_map *map, json_object_entry js)
{
	if (!ft_strcmp(js.name, "vertex"))
	{
		if (vertex_field(map, js.value))
			return (1);
	}
	else if (!ft_strcmp(js.name, "sector"))
	{
		if (map->sectors || !map->vertices ||
			!map->number_vertices || !js.value ||
			js.value->type != json_array || sector_field(map, js.value))
			return (1);
	}
	else if (!ft_strcmp(js.name, "sprite"))
	{
		if (sprite_field(map, js.value))
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
	size_t			i;

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

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_stat_init(&st);
	zip_stat(z, file_name, 0, &st);
	if (st.size < 1)
		MSG("No such map");
	file_contents = malloc(st.size);
	f = zip_fopen_encrypted(z, file_name, 0, RESOURCES_PASS);
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
	char			*full_name;
	json_value		*value;

	map->number_vertices = 0;
	map->number_sectors = 0;
	map->number_sprites = 0;
	map->number_enemies = 0;
	name = ft_strjoin("assets/maps/", map_name);
	full_name = ft_strjoin(name, ".json");
	if (!(value = init_json(full_name)) || parser_loop(map, value))
		MSG("Wrong data format");
	json_value_free(value);
	free(name);
	free(full_name);
	return (0);
}
