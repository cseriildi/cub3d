/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_texture.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:26:07 by icseri            #+#    #+#             */
/*   Updated: 2024/11/20 17:03:15 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_texture(char *line, t_data *data)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
	{
		if (data->map.north)
		{
			free(line);
			print_error(1, "Error\nDuplicate north texture");
			safe_exit(data, EXIT_FAILURE);
		}
		data->map.north = ft_strdup(line + 3);
		free(line);
		if (!data->map.north)
		{
			print_error(1, "Error\nMalloc fail");
			safe_exit(data, EXIT_FAILURE);
		}
	}
}
