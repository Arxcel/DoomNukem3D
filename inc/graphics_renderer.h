/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_renderer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 13:57:54 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef graphics_renderer_H
# define graphics_renderer_H
# include "structure.h" 

# define NEAR_Z     1e-4f
# define FAR_Z      5
# define NEAR_SIDE  1e-5f
# define FAR_SIDE   20.f

typedef struct		s_renderItem
{
    int sectorno;
    int limitXLeft;
    int limitXRight;
}                   t_renderItem;

typedef struct      s_renderer
{
    t_renderItem    queue[MaxQueue];
    t_renderItem    *head;
    t_renderItem    *tail;
    int             *renderedSectors;
    int             *topLimit;
    int             *bottomLimit;
    t_vector        t1;
    t_vector        t2;

}                   t_renderer;
    
typedef struct      s_vline
{
    int             x;
    int             yTop;
    int             yBottom;
    int             colorTop;
    int             colorMain;
    int             colorBottom;
}                   t_vline;

typedef struct      s_wall
{
    int             x1;
    int             y1[2];
    int             x2;
    int             y2[2];
    t_vertex        scale1;
    t_vertex        scale2;
    
    float           ceil;
    float           floor;

    int             neighbor;
    float           neighbor_ceil;
    float           neighbor_floor;
    int             neighbor_y1[2];
    int             neighbor_y2[2];

}                   t_wall;

#endif