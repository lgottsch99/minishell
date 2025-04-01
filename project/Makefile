# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 15:57:29 by lgottsch          #+#    #+#              #
#    Updated: 2025/04/01 14:27:38 by lgottsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

LIBFTDIR = ./lib
LIBFTA = $(LIBFTDIR)/libfull_libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I/opt/homebrew/opt/readline/include
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline # -fsanitize=address

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), access.c alloc_free_exec.c builtin_cd.c builtin_echo.c builtin_exit.c \
 	builtin_exit_helpers.c builtin_export.c builtin_export_helpers.c \
 	builtin_unset.c builtins.c child_helpers.c child.c child_redirection.c \
 	env_helpers.c env_helpers2.c env_setup.c exec_path_helpers.c exec_path.c \
 	execution.c execution_helpers.c execution_helpers2.c free_helpers.c heredoc_helpers.c init_start.c \
 	main.c parsing.c redirections.c signals_helpers.c single_builtin.c tokenising.c \
 	tokens_handlers.c tokens_handlers2.c tokens_handlers3.c tokens_handlers4.c env_tokenising.c \
 	word_tokenising.c parsing_helpers2.c parsing_helpers.c)

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
	@make -C $(LIBFTDIR) clean

re: fclean all

.PHONY: all clean fclean re libft

# NAME = minishell
# LIBFTDIR= ./lib
# LIBFTA= lib/libfull_libft.a
# CC = cc
# CFLAGS = -Wall -Wextra -Werror -lreadline
# CFMAC = -L /Users/$(USER)/.brew/opt/readline/lib
# SRC_FOLDER = ./src/
# SRC = $(addprefix $(SRC_FOLDER), access.c alloc_free_exec.c builtin_cd.c builtin_echo.c builtin_exit.c \
#  	builtin_exit_helpers.c builtin_export.c builtin_export_helpers.c \
#  	builtin_unset.c builtins.c child_helpers.c child.c child_redirection.c \
#  	env_helpers.c env_helpers2.c env_setup.c exec_path_helpers.c exec_path.c \
#  	execution.c execution_helpers.c free_helpers.c heredoc_helpers.c init_start.c \
#  	main.c parsing.c redirections.c signals_helpers.c single_builtin.c tokenising.c \
#  	tokens_handlers.c tokens_handlers2.c tokens_handlers3.c tokens_handlers4.c env_tokenising.c \
#  	word_tokenising.c parsing_helpers2.c parsing_helpers.c)


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
