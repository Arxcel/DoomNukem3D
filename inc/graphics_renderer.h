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


typedef struct  s_renderer
{
    t_renderItem    queue[MaxQueue];
    t_renderItem    *head;
    t_renderItem    *tail;
    int             *renderedSectors;
    int             *topLimit;
    int             *bottomLimit;

}               t_renderer;
    
typedef struct  s_line
{
    int x;
    int yTop;
    int yBottom;
    int colorTop;
    int colorMain;
    int colorBottom;
}               t_line;
#endif