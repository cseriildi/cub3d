/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/04 17:23:24 by icseri           ###   ########.fr       */
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
	data->north = NULL;
	data->south = NULL;
	data->west = NULL;
	data->east = NULL;
	data->map = NULL;
	data->row = 0;
	data->column = 0;
	data->floor = NULL;
	data->ceiling = NULL;
	data->fd = -1;
}
