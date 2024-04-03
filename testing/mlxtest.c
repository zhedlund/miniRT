// compile: cc mlxtest.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext

#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include "minilibx-linux/mlx.h"


int main(void)
{
    void	*mlx_ptr;

    mlx_ptr = mlx_init();
    mlx_destroy_display(mlx_ptr);
    free(mlx_ptr);
}
