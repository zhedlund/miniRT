t_color ray_color(const t_ray *r, const t_scene *scene)
{
    float ts = hit_sphere(&((const t_sph*)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, r);
    float tp = hit_plane((const t_plane*)(scene->objs->next->obj), r);
    t_color px;

	px = (t_color){0.5, 0.7, 1.0}; // Background color
    if (ts > 0 && (tp < 0 || ts < tp)) // Intersection with the sphere
    { 
        // Calculate intersection point and normal vector
        t_vec intersect = intersect_point(r, ts);
        t_vec normal = sphere_normal((const t_sph*)(scene->objs->obj), &intersect);
        t_color ambient_light = ambient_color(&scene->a, &((const t_sph*)(scene->objs->obj))->color);

        // Diffuse lighting for shadow effect
        t_vec light_dir = vec3_unit_vector(&scene->l.pos);
        float diffuse_factor = dot(&light_dir, &normal);
        t_color diffuse = diffuse_color(&scene->l, &((const t_sph*)(scene->objs->obj))->color, diffuse_factor);
        px = blend_color(&ambient_light, &diffuse); // add ambient and diffuse components
    } 
    else if (tp > 0) // Intersection with the plane
    {
        t_vec intersect_plane = intersect_point(r, tp);
        // Check if the sphere is blocking the light
        t_vec temp = vec3_subtract(scene->l.pos, intersect_plane);
        t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source

        t_ray shadow_ray = {intersect_plane, to_light};
        float t_shadow_sphere = hit_sphere(&((const t_sph*)(scene->objs->obj))->center, ((const t_sph*)(scene->objs->obj))->radius, &shadow_ray);
        if (t_shadow_sphere > 0 && t_shadow_sphere < 1.0) // Sphere is blocking the light
        {
            px = (t_color){0.0, 0.3, 0.2}; // shadow color
        }
        else // No shadow
        {
            t_color ambient_light = ambient_color(&scene->a, &((const t_plane*)(scene->objs->next->obj))->color); // Ambient lighting for plane
            px = ambient_light; // Plane pixel color with ambient component
        }
    }
    return (px);
}
