# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: Watanudon <Watanudon@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/02 14:43:32 by lgottsch          #+#    #+#              #
#    Updated: 2024/12/19 23:10:26 by Watanudon        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_FOLDER= ./src/
SRC= 	$(addprefix $(SRC_FOLDER), \
		ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c \
					ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c \
					ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
					ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c\
					 ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
					 ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
					 ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c \
					 ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c \
					 ft_tolower.c ft_toupper.c \
					 ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
					 ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstnew.c \
					 ft_lstsize.c \
					./printf/ft_memaddress.c ./printf/ft_printf.c \
					./printf/pf_putchar.c ./printf/pf_putnbr.c \
					./printf/ft_putnbr_base.c ./printf/pf_putstr.c \
					./printf/ft_putuint.c \
					./gnl/get_next_line.c ./gnl/get_next_line_utils.c )
									

OBJ = $(SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

NAME = libfull_libft.a

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean:
	rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
