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

#ifndef FT_RT_H
# define FT_RT_H

# include "ft_sdl.h"

# define WIN_W			1280
# define WIN_H			670

typedef struct		s_main
{
	t_sdl	sdl;
}					t_main;

void				sdl_hook(t_main *m);
void				sdl_loop(t_main *m);

#endif
