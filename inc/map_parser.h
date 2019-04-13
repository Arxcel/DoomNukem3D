/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sahafono <sahafono@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/13 12:40:50 by sahafono          #+#    #+#             */
/*   Updated: 2019/04/13 12:40:52 by sahafono         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_PARSER_H
# define MAP_PARSER_H
# include "doom_nukem.h"
# include "json.h"

int				parser(t_map *map, char *file_name);
int				remove_data(t_map *map);
int				remove_values(t_map *map, json_value *value);
int				player_field(t_map *map, json_value *value);
int				sector_field(t_map *map, json_value *value);
int				count_vertices(t_map *map, json_value *value);

#endif
