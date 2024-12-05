/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 15:42:11 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_texture(char *line, char **texture, t_map *map)
{
	if (*texture)
	{
		ft_free(&line);
		print_error(1, "Error\nDuplicate texture");
		safe_exit(map, EXIT_FAILURE);
	}
	if (ft_strlen(line) <= 3)
	{
		ft_free(&line);
		print_error(1, "Error\nInvalid texture");
		safe_exit(map, EXIT_FAILURE);
	}
	*texture = ft_substr(line, 3, ft_strlen(line) - 4);
	ft_free(&line);
	if (!*texture)
	{
		print_error(1, "Error\nMalloc fail");
		safe_exit(map, EXIT_FAILURE);
	}
}
