# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lgottsch <lgottsch@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/26 15:57:29 by lgottsch          #+#    #+#              #
#    Updated: 2025/02/14 15:41:47 by lgottsch         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME = minishell

LIBFTDIR = ./libft
LIBFTA = $(LIBFTDIR)/libfull_libft.a

CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
CFMAC = -L /Users/$(USER)/.brew/opt/readline/lib

# Add ARCH variable (default to arm64 if not specified)
ARCH ?= arm64
ARCH_FLAGS = -arch $(ARCH)

SRC_FOLDER = ./src/
SRC = $(addprefix $(SRC_FOLDER), main.c init_start.c builtins.c execution.c access_exec.c redirections.c \
      single_builtin.c alloc_free_exec.c export.c unset.c parsing.c tokenising.c)

OBJ = $(SRC:.c=.o)

$(NAME): $(OBJ) $(LIBFTA)
	$(CC) $(CFLAGS) $(ARCH_FLAGS) $(OBJ) $(LIBFTA) $(LDFLAGS) -o $(NAME)

all: $(NAME)

libft:
	@make -C $(LIBFTDIR) ARCH=$(ARCH)

clean:
	rm -f $(OBJ)
	@cd $(LIBFTDIR) && make clean

fclean: clean
	rm -f $(NAME)
	@cd $(LIBFTDIR) && make fclean

re: fclean all

mac: libft
	$(CC) $(CFLAGS) $(ARCH_FLAGS) $(CFMAC) $(SRC) $(LIBFTA) $(LDFLAGS) -o $(NAME)

.PHONY: all clean fclean re libft