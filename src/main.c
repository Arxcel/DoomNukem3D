/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/21 16:33:57 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/23 14:13:08 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doomNukem.h"

int					main(int ac, char **av)
{
	t_main			m;
	(void)av;
	ft_bzero(&m, sizeof(t_main));
	
	m.sdl.win_w = WIN_W;
	m.sdl.win_h = WIN_H;
	
	sdl_init(&m.sdl);
	sdl_loop(&m);
	return (0);
}
