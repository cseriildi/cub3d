/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/11/20 17:02:39 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_data(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	parsing(argc, argv, &data);
}

void	init_data(t_data *data)
{
	data->map.north = NULL;
	data->map.south = NULL;
	data->map.west = NULL;
	data->map.east = NULL;
	data->map.map = NULL;
	data->fd = -1;
}
