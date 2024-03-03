FILES := commands.c \
		 utils.c \
		 first_input.c \
		 pipex.c
MANDATORY := main.c
BONUS := main_bonus.c

SRCDIR := src
SRC := $(addprefix $(SRCDIR)/,$(FILES))
SRC_M := $(SRC) $(MANDATORY)
SRC_B := $(SRC) $(BONUS)

BUILDDIR := build
OBJ := $(addprefix $(BUILDDIR)/,$(FILES:.c=.o))
MANDATORY_OBJ := $(addprefix $(BUILDDIR)/,$(MANDATORY:.c=.o))
BONUS_OBJ := $(addprefix $(BUILDDIR)/,$(BONUS:.c=.o))

OBJ_M := $(OBJ) $(MANDATORY_OBJ)
OBJ_B := $(OBJ) $(BONUS_OBJ)

CC := cc
CFLAGS := -Wall -Wextra -Werror
GDB := -g3
export GDB

LIBFTDIR := libft
LIBFT := ft
LIBFTFILE := lib$(LIBFT).a

GNLDIR := gnl
GNL := gnl
GNLFILE := lib$(GNL).a

NAME := pipex
NAME_BONUS := $(NAME)_bonus

.PHONY: all bonus
all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(LIBFTDIR)/$(LIBFTFILE) $(GNLDIR)/$(GNLFILE) $(OBJ_M)
	$(CC) $(CFLAGS) $(GDB) -I. -Iinc -o $@ $(OBJ_M) -L$(LIBFTDIR) -l$(LIBFT) -L$(GNLDIR) -l$(GNL)

$(NAME_BONUS): $(LIBFTDIR)/$(LIBFTFILE) $(GNLDIR)/$(GNLFILE) $(OBJ_B)
	$(CC) $(CFLAGS) $(GDB) -I. -Iinc -o $@ $(OBJ_B) -L$(LIBFTDIR) -l$(LIBFT) -L$(GNLDIR) -l$(GNL)

$(LIBFTDIR)/$(LIBFTFILE):
	make -C $(LIBFTDIR)

$(GNLDIR)/$(GNLFILE):
	make -C $(GNLDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(GDB) -I. -Iinc -c $< -o $@

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(GDB) -I. -Iinc -c $< -o $@

$(BUILDDIR):
	@mkdir -p $@


.PHONY: clean fclean re norm
clean:
	rm -f $(OBJ) $(MANDATORY_OBJ) $(BONUS_OBJ)
	rm -rf $(BUILDDIR)
	make -C $(GNLDIR) clean
	make -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)
	make -C $(GNLDIR) fclean
	make -C $(LIBFTDIR) fclean

re: fclean all

norm:
	make -C $(LIBFTDIR) norm
	make -C $(GNLDIR) norm
	norminette -R CheckForbiddenSourceHeader $(SRC) $(MANDATORY) $(BONUS)
	norminette -R CheckDefine inc/
