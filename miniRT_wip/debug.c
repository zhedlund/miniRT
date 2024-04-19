/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 18:08:30 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/19 18:09:27 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

const char* shape_to_string(t_shape shape)
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

void print_object_list(const t_obj *head)
{
    const t_obj *current = head;
	
    while (current != NULL)
	{
        const char* shape_str = shape_to_string(current->id);
        write(STDOUT_FILENO, "Object ID: ", 11);
        write(STDOUT_FILENO, shape_str, ft_strlen(shape_str));
        current = current->next;
    }
}