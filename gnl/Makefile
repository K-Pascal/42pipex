SRC := get_next_line.c \
	   get_next_line_utils.c
OBJ := $(SRC:.c=.o)

CC := cc
CFLAGS := -Wall -Wextra -Werror
NAME := libgnl.a

GDB ?= 

.PHONY: all
all : $(NAME)

$(NAME): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(GDB) -I. -c $< -o $@

.PHONY: clean fclean re norm
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

norm:
	norminette -R CheckForbiddenSourceHeader $(SRC)
	norminette -R CheckDefine get_next_line.h
