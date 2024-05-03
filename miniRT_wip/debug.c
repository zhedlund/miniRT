/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 18:08:30 by zhedlund          #+#    #+#             */
/*   Updated: 2024/05/03 18:57:58 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char* shape_to_string(t_shape shape)
{
    switch (shape)
	{
        case SPHERE:
            return "SPHERE\n";
        case PLANE:
            return "PLANE\n";
        case CYLINDER:
            return "CYLINDER\n";
        default:
            return "UNKNOWN\n";
    }
}

void print_object_list(t_obj *head)
{
    t_obj *current = head;
	
    while (current != NULL)
	{
        char* shape_str = shape_to_string(current->id);
        write(STDOUT_FILENO, "Object ID: ", 11);
        write(STDOUT_FILENO, shape_str, ft_strlen(shape_str));
        current = current->next;
    }
}