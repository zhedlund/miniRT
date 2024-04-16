# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: zhedlund <zhedlund@student.42berlin.de>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/16 11:57:37 by zhedlund          #+#    #+#              #
#    Updated: 2024/04/16 13:32:12 by zhedlund         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LFLAGS = -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm
#LIBFTDIR = ./libft
#LIBFT = $(LIBFTDIR)/libft.a
COMPILED = .compiled

SRC = 	main.c \
		color.c \
		objects.c \
		ray_color.c \
		mlx_functions.c \
		math/math.c \
		math/math_vector.c \
		utils/ft_atof.c \
		
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

.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ) $(COMPILED)
#@make -s -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME) $(COMPILED)
#@make -s -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re progress