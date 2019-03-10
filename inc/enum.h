/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enum.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/09 11:30:14 by vkozlov           #+#    #+#             */
/*   Updated: 2019/03/09 11:30:14 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUM_H
# define ENUM_H

enum	e_move_dir
{
	Nothing = 0,
	Forward,
	Backward,
	Right,
	Left,
};

enum
{
	MaxQueue = 32
};


#endif
