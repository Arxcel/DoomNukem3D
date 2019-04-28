/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:44:28 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/28 11:22:56 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int		remove_values(t_map *map, json_value *value)
{
	size_t i;

	if (value)
		json_value_free(value);
	if (map->vertices)
		free(map->vertices);
	if (map->sectors)
	{
		i = 0;
		while (i < map->number_sectors)
		{
			if (map->sectors[i].vertices)
				free(map->sectors[i].vertices);
			if (map->sectors[i].neighbors)
				free(map->sectors[i].neighbors);
			if (map->sectors[i].textures)
				free(map->sectors[i].textures);
			i++;
		}
		free(map->sectors);
	}
	return (1);
}

int		remove_data(t_map *map)
{
	remove_values(map, NULL);
	return (0);
}
