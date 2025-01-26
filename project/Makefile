# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 15:57:29 by lgottsch          #+#    #+#              #
#    Updated: 2025/01/26 18:13:57 by lgottsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFTDIR= ./lib
LIBFTA= lib/libfull_libft.a

CC = cc 
CFLAGS = -Wall -Wextra -Werror -lreadline

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), main.c )

OBJ = $(SRC:.c=.o)

$(NAME): libft
	$(CC) $(CFLAGS) $(SRC) $(LIBFTA) -o $(NAME)

all: $(NAME)

libft:
	@make -C lib

clean:
	rm -f $(OBJ)
	@cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	@cd $(LIBFTDIR) && make clean && make fclean

re: fclean all

.PHONY: all clean fclean re libft