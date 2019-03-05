/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:32:12 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 13:57:54 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef math_utils_h
# define math_utils_h
#include "structure.h"

float minf(float a, float b);
float maxf(float a, float b);
float clampf(float a, float min, float max);
float crossf(float x0, float y0, float x1, float y1);
int overlapf(float begin0, float end0, float begin1, float end1);
int intersectBox(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4);
t_vertex intersectLine(t_vertex v1, t_vertex v2, t_vertex v3, t_vertex v4);
float pointSide(t_vertex p, t_vertex v1, t_vertex v2);

#endif