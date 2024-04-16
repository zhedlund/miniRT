/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:11:27 by kdzhoha           #+#    #+#             */
/*   Updated: 2024/04/16 15:25:39 by kdzhoha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_array(char **str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
}

void	free_scene(t_scene *scene)
{
	t_obj	*cur;
	t_obj	*next;

	if (!scene)
		return ;
	cur = scene->objs;
	while (cur)
	{
		next = cur->next;
		free(cur->obj);
		free(cur);
		cur = next;
	}
	free(scene);
}
