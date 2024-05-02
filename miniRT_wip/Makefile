# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kdzhoha <kdzhoha@student.42berlin.de >     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 11:57:37 by zhedlund          #+#    #+#              #
#    Updated: 2024/05/02 19:41:12 by kdzhoha          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm
DEPS = parsing/parsing.h minirt.h
#LIBFTDIR = ./libft
#LIBFT = $(LIBFTDIR)/libft.a
COMPILED = .compiled

SRC = 	main.c \
		color.c \
		objects.c \
		rendering.c \
		hit_cylinder.c \
		math/math.c \
		math/math_vector.c \
		utils/list_utils.c \
		utils/mlx_utils.c \
		debug.c \
		parsing/parse_input.c \
		parsing/input_check.c \
		parsing/add_object.c \
		parsing/utils.c \
		parsing/free.c \
		parsing/ft_split.c \
		parsing/ft_atof.c \
		parsing/get_next_line.c \
		parsing/get_next_line_utils.c \
		parsing/errors.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	@printf "Compiling: [\033[0;32m"
	@$(MAKE) progress -s
	@printf "\033[0m] Done\n"
	@$(CC) $(OBJ) $(LFLAGS) -o $(NAME)
#-L$(LIBFTDIR) -lft
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
#@make -s -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME) $(COMPILED)
#@make -s -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re progress
