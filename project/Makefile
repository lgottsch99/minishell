# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 15:57:29 by lgottsch          #+#    #+#              #
#    Updated: 2025/03/07 13:19:51 by Watanudon        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFTDIR = ./lib
LIBFTA = $(LIBFTDIR)/libfull_libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline -fsanitize=address

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), main.c init_start.c builtins.c execution.c access_exec.c redirections.c \
      single_builtin.c alloc_free_exec.c export.c unset.c parsing.c tokenising.c heredoc_helpers.c \
      env_helpers.c signals_helpers.c)

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ) libft
	$(CC) $(CFLAGS) $(OBJ) $(LIBFTA) $(LDFLAGS) -o $(NAME)

all: $(NAME)

libft:
	@make -C $(LIBFTDIR)

clean:
	rm -f $(OBJ)
	@make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re libft