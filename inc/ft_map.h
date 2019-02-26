/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doomNukem.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 13:57:54 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_MAP_H
# define FT_MAP_H

# include <player.h>

typedef struct		s_vertex
{
    float           x;
    float           y;
}					t_vertex;

typedef struct		s_sector
{
    float           floorHeight;
    float           ceilHeight;
    short           *neighbors;
    size_t          numberNeighbors;
    t_vertex        *vertices;
    size_t          numberSectorVertices;
}                   t_sector;

typedef struct		s_enemy
{
    t_vector        position;
    t_vector        velocity;
    float           angle;
    float           yaw;
    float           anglesin;
    float           anglecos;
    size_t          sectorNumber;
}                   t_enemy;

typedef struct		s_sprite
{
    t_vector        position;
    float           angle;
    float           yaw;
    float           anglesin;
    float           anglecos;
    size_t          sectorNumber;
}                   t_sprite;

typedef struct		s_map
{
	t_vertex        *vertices;
    t_sector        *sectors;
    t_enemy         *enemies;
    t_sprite        *sprites;
    t_player        player;
    size_t          numberVertices;
    size_t          numberSectors;
    size_t          numberEnemies;
    size_t          numberSptites;
}					t_map;

#endif
