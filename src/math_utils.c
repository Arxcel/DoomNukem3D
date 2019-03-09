/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:47:26 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:47:29 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "math_utils.h"

float		minf(float a, float b)
{
	if (a < b)
		return (a);
	return (b);
}

float		maxf(float a, float b)
{
	if (a > b)
		return (a);
	return (b);
}

float		clampf(float a, float min, float max)
{
	return (minf(maxf(a, min), max));
}
