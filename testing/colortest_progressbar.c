#include <stdio.h>
#include <unistd.h>

size_t	ft_strlen(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}


int main() {
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render
    printf("P3\n%d %d\n255\n", image_width, image_height);

    int j = 0;
    while (j < image_height)
    {
        int i = 0;
        while (i < image_width) {
            double r = (double)i / (image_width - 1);
            double g = (double)j / (image_height - 1);
            double b = 0;

            int ir = (int)(255.999 * r);
            int ig = (int)(255.999 * g);
            int ib = (int)(255.999 * b);

            printf("%d %d %d\n", ir, ig, ib);
            i++;
        }
        j++;
    }
    write(2, "Done.\n", 6);
    return 0;
}




