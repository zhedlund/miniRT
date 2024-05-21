# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 11:57:37 by zhedlund          #+#    #+#              #
#    Updated: 2024/05/21 16:19:19 by kdzhoha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O3
LFLAGS = -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm
DEPS = parsing/parsing.h minirt.h

SRC = 	main.c \
		color.c \
		objects.c \
		rendering.c \
		hit_cylinder.c \
		compute_lighting.c \
		viewport.c \
		math/math.c \
		math/math_vector.c \
		utils/list_utils.c \
		utils/mlx_utils.c \
		utils/rendering_utils.c \
		debug.c \
		parsing/parse_input.c \
		parsing/input_check.c \
		parsing/input_check_2.c \
		parsing/add_object.c \
		parsing/add_sphere.c \
		parsing/add_cylinder.c \
		parsing/utils.c \
		parsing/free.c \
		parsing/ft_split.c \
		parsing/ft_atof.c \
		parsing/get_next_line.c \
		parsing/get_next_line_utils.c \
		parsing/errors.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@printf "Compiling: [\033[0;32m"
	@$(MAKE) progress -s
	@printf "\033[0m] Done\n"
	@$(CC) $(OBJ) $(LFLAGS) -o $(NAME)

progress:
	@for i in $(shell seq 1 $(words $(SRC))); do \
		sleep 0.1; \
		printf "="; \
	done

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re progress
