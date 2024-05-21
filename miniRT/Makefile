# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 11:57:37 by zhedlund          #+#    #+#              #
#    Updated: 2024/05/21 23:35:20 by zhedlund         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -O3
LFLAGS = -L../minilibx-linux -lmlx_Linux -lX11 -lXext -lm
DEPS = parsing/parsing.h minirt.h
COMPILED = .compiled

SRC = 	main.c \
		math/math.c \
		math/math_vector.c \
		objects/objects.c \
		objects/hit_cylinder.c \
		render/color.c \
		render/lighting.c \
		render/rendering.c \
		render/viewport.c \
		utils/mlx_utils.c \
		utils/rendering_utils.c \
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

all: libmlx check_changes $(NAME)

libmlx:
	@$(MAKE) -C ../minilibx-linux > /dev/null

check_changes:
	@if [ -e $(COMPILED) ] && [ -z "$$(find $(SRC) -newer $(COMPILED))" ]; then \
		echo "make: Nothing to be done for 'all'."; \
	fi

$(NAME): $(OBJ)
	@printf "Compiling: [\033[0;32m"
	@$(MAKE) progress -s
	@printf "\033[0m] Done\n"
	@$(CC) $(OBJ) $(LFLAGS) -o $(NAME)
	@touch $(COMPILED)

progress:
	@for i in $(shell seq 1 $(words $(SRC))); do \
		sleep 0.1; \
		printf "="; \
	done

%.o: %.c $(DEPS)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ) $(COMPILED)
	
fclean: clean
	@rm -f $(NAME) $(COMPILED)

re: fclean all

.PHONY: all clean fclean re progress check_changes
