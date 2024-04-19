/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 11:42:15 by zhedlund          #+#    #+#             */
/*   Updated: 2024/04/19 18:47:38 by zhedlund         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minirt.h"


/*void add_object(t_scene* scene, t_obj* obj)
{
    t_obj* node;
	
	node = (t_obj*)malloc(sizeof(t_obj));
    if (!node)
	{
		write(1, "Error\nMemory allocation failed\n", 31);
        return ;
    }
    node->id = obj->id;
    node->obj = obj->obj;
    node->next = scene->objs;
    scene->objs = node;
}*/

void add_object(t_scene* scene, t_obj* obj)
{
    t_obj	*node = (t_obj*)malloc(sizeof(t_obj));
	t_obj	*current;
	
    if (!node)
    {
        write(1, "Error\nMemory allocation failed\n", 31);
        return ;
    }
    node->id = obj->id;
    node->obj = obj->obj;
    node->next = NULL;
    if (scene->objs == NULL)
        scene->objs = node; // If the list is empty, make the new node the head
	else // find last node
	{
        current = scene->objs;
        while (current->next != NULL)
            current = current->next;
        current->next = node; // add node to the end of the list
    }
}


void free_obj_list(t_scene* scene)
{
    t_obj *current;
	t_obj *next;
	
	current = scene->objs;
    while (current != NULL)
	{
        next = current->next;
        free(current);
        current = next;
    }
    scene->objs = NULL;
}

void free_hitlist(t_hit* hit)
{
	t_hit *current;
	t_hit *next;
	
	current = hit;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	hit = NULL;
}