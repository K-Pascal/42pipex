FILES := main.c

SRCDIR := src
BUILDDIR := build
SRC := $(addprefix $(SRCDIR)/,$(FILES))
OBJ := $(addprefix $(BUILDDIR)/,$(FILES:.c=.o))

CC := cc
CFLAGS := -Wall -Wextra -Werror
GDB := -g3
export GDB

LIBFTDIR := libft
LIBFTFILE := libft.a
LIBFT := ft

GNLDIR := gnl
GNLFILE := libgnl.a
GNL := gnl

NAME := pipex

.PHONY: all
all: $(NAME)

$(NAME): $(LIBFTDIR)/$(LIBFTFILE) $(GNLDIR)/$(GNLFILE) $(OBJ)
	$(CC) $(CFLAGS) $(GDB) -I. -o $@ $^ -L$(LIBFTDIR) -l$(LIBFT) -L$(GNLDIR) -l$(GNL)

$(LIBFTDIR)/$(LIBFTFILE):
	make -C $(LIBFTDIR)

$(GNLDIR)/$(GNLFILE):
	make -C $(GNLDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p build/
	$(CC) $(CFLAGS) $(GDB) -I. -c $< -o $@

.PHONY: clean fclean re norm
clean:
	rm -f $(OBJ)
	make -C $(GNLDIR) clean
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(GNLDIR) fclean
	make -C $(LIBFTDIR) fclean

re: fclean all

norm:
	make -C $(LIBFTDIR) norm
	make -C $(GNLDIR) norm
	norminette -R CheckForbiddenSourceHeader $(SRC)
	norminette -R CheckDefine inc/
