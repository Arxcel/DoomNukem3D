/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/29 15:42:50 by vkozlov           #+#    #+#             */
/*   Updated: 2019/04/29 16:37:56 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "doom_nukem.h"

int					write_map_to_file(char *buf, const char *filename)
{
	zip_t				*z;
	zip_source_t		*s;
	zip_int64_t			index;
	int					res;
	zip_error_t			e;

	z = zip_open(RESOURCES, ZIP_CREATE, 0);
	if (!z)
		MSG(zip_strerror(z));
	zip_set_default_password(z, RESOURCES_PASS);
	s = zip_source_buffer_create(buf, ft_strlen(buf), 0, &e);
	if (!s)
		MSG(e.str);
	index = zip_name_locate(z, filename, 0);
	if (index < 0)
		MSG(zip_strerror(z));
	zip_delete(z, index);
	res = zip_file_add(z, filename, s, ZIP_FL_ENC_UTF_8);
	if (res < 0)
	{
		zip_source_free(s);
		MSG(zip_strerror(z));
	}
	zip_close(z);
	return (0);
}
