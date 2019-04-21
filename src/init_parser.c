/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:36:42 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/13 13:34:35 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map_parser.h"
#include <sys/stat.h>

int						vertex_field(t_map *map, json_value *value)
{
	int			cnt;
	json_value	*js;

	if (value->type != json_array || value->u.array.length < 3 ||
	!(map->vertices = (t_vertex*)malloc(sizeof(t_vertex) * value->u.array.length)))
		return (1);
	map->number_vertices = value->u.array.length;
	cnt = -1;
	while (++cnt < value->u.array.length)
	{
		js = value->u.array.values[cnt];
		if (js->type != json_object || ft_strcmp(js->u.object.values[0].name, "x")
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
	FILE		*fp;
	struct stat	filestatus;
	size_t		file_size;
	char		*file_contents;
	json_value	*value;

	value = NULL;
	if (stat(file_name, &filestatus) != 0)
		MSG("File %s not found\n");
	file_size = (size_t)filestatus.st_size;
	if (!(fp = fopen(file_name, "rt")))
		MSG("Unable to open file\n");
	if (!(file_contents = (char*)malloc(file_size)))
		MSG("Memory error: unable to allocate memory\n");
	if (fread(file_contents, file_size, 1, fp) != 1 || fclose(fp))
	{
		free(file_contents);
		MSG("Unable to read file\n");
	}
	if (!(value = json_parse(file_contents, file_size)))
		ft_putendl("Unable to parse data");
	free(file_contents);
	return (value);
}

int						parser(t_map *map, char *file_name)
{
	json_value	*value;

	map->number_vertices = 0;
	map->number_sectors = 0;
	map->number_sptites = 0;
	map->number_enemies = 0;
	if (!(value = init_json(file_name)) || parser_loop(map, value))
		MSG("Wrong data format");
	json_value_free(value);
	return (0);
}
