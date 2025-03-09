# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 15:57:29 by lgottsch          #+#    #+#              #
#    Updated: 2025/03/08 16:15:15 by lgottsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFTDIR = ./lib
LIBFTA = $(LIBFTDIR)/libfull_libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline # -fsanitize=address

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), main.c init_start.c builtins.c execution.c access_exec.c redirections.c \
      single_builtin.c alloc_free_exec.c builtin_export.c builtin_unset.c parsing.c tokenising.c heredoc_helpers.c \
      env_helpers.c signals_helpers.c builtin_echo.c builtin_exit.c)

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ) #libft
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFTA) $(LDFLAGS) -o $(NAME)
	@make clean

all: $(NAME)

libft:
	@make -C $(LIBFTDIR)

clean:
	@rm -f $(OBJ)
	@make -C $(LIBFTDIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re libft

# NAME = minishell
# LIBFTDIR= ./lib
# LIBFTA= lib/libfull_libft.a
# CC = cc
# CFLAGS = -Wall -Wextra -Werror -lreadline
# CFMAC = -L /Users/$(USER)/.brew/opt/readline/lib
# SRC_FOLDER = ./src/
# SRC = $(addprefix $(SRC_FOLDER), main.c init_start.c builtins.c execution.c access_exec.c redirections.c \
#       single_builtin.c alloc_free_exec.c builtin_export.c builtin_unset.c parsing.c tokenising.c heredoc_helpers.c \
#       env_helpers.c signals_helpers.c builtin_echo.c builtin_exit.c)
# OBJ = $(SRC:.c=.o)

# $(NAME): libft
# 	$(CC) $(CFLAGS) $(SRC) $(LIBFTA) -o $(NAME)
# 	make clean

# all: $(NAME)

# libft:
# 	@make -C lib

# clean:
# 	rm -f $(OBJ)
# 	@cd $(LIBFTDIR) && make clean

# fclean: clean
# 	rm -f $(NAME)
# 	@cd $(LIBFTDIR) && make clean && make fclean

# re: fclean all

# mac: libft
# 	$(CC) $(CFLAGS) $(CFMAC) $(SRC) $(LIBFTA) -o $(NAME)
	
# .PHONY: all clean fclean re libft mac