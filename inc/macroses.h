/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macroses.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmaluh <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 16:54:00 by tmaluh            #+#    #+#             */
/*   Updated: 2019/03/21 19:08:32 by tmaluh           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACROSES_H
# define MACROSES_H

# define _NOTIS_F(ex) if (!(ex)) return (false)
# define _Z(type, dest, x) ft_bzero(dest, sizeof(type) * (x))
# define _ISZ(t, d, x) _NOTIS_F(d = (t*)malloc(sizeof(t)*(x))); _Z(t, d, x)

#endif
