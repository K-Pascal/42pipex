RED		:=\e[31m
GREEN	:=\e[32m
ORANGE	:=\e[33m
BLUE	:=\e[34m
PURPLE	:=\e[35m
CYAN	:=\e[36m

BOLD	:=\e[1m
DIM		:=\e[2m
BOM		:=\e[22m
ITALIC	:=\e[3m
NOITA	:=\e[23m

GOTO_B	:=\e[1A\e[K

DEFAULT	:=\e(B\e[m

FILES		:=	commands.c		\
				utils.c			\
				first_input.c	\
				pipex.c

MANDATORY	:=	main.c
BONUS		:=	main_bonus.c

SRCDIR	:=	src
SRC		:=	$(addprefix $(SRCDIR)/,$(FILES))
SRC_M	:=	$(SRC) $(MANDATORY)
SRC_B	:=	$(SRC) $(BONUS)

BUILDDIR		:=	build
OBJ				:=	$(addprefix $(BUILDDIR)/,$(FILES:.c=.o))
MANDATORY_OBJ	:=	$(addprefix $(BUILDDIR)/,$(MANDATORY:.c=.o))
BONUS_OBJ		:=	$(addprefix $(BUILDDIR)/,$(BONUS:.c=.o))

OBJ_M	:=	$(OBJ) $(MANDATORY_OBJ)
OBJ_B	:=	$(OBJ) $(BONUS_OBJ)

CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
GDB		:=
export GDB

LIBFTDIR	:=	libft
LIBFT		:=	ft
LIBFTFILE	:=	lib$(LIBFT).a

GNLDIR	:=	gnl
GNL		:=	gnl
GNLFILE	:=	lib$(GNL).a

NAME		:=	pipex
NAME_BONUS	:=	$(NAME)_bonus

.PHONY: all bonus
all: $(NAME)

bonus: $(NAME_BONUS)

$(NAME): $(LIBFTDIR)/$(LIBFTFILE) $(GNLDIR)/$(GNLFILE) $(OBJ_M)
	@echo "Creating the executable $(BOLD)$@$(DEFAULT)"
	@$(CC) $(CFLAGS) $(GDB) -I. -Iinc -o $@ $(OBJ_M) -L$(LIBFTDIR) -l$(LIBFT) -L$(GNLDIR) -l$(GNL)\
		&& (echo "$(GOTO_B)$(GREEN)Sucessfully linked into $(BOLD)$@$(DEFAULT)")


$(NAME_BONUS): $(LIBFTDIR)/$(LIBFTFILE) $(GNLDIR)/$(GNLFILE) $(OBJ_B)
	@echo "Creating the executable $(BOLD)$@$(DEFAULT)"
	@$(CC) $(CFLAGS) $(GDB) -I. -Iinc -o $@ $(OBJ_B) -L$(LIBFTDIR) -l$(LIBFT) -L$(GNLDIR) -l$(GNL)\
		&& (echo "$(GOTO_B)$(GREEN)Sucessfully linked into $(BOLD)$@$(DEFAULT)")

$(LIBFTDIR)/$(LIBFTFILE):
	@echo "Creating the library $(ITALIC)$(LIBFTFILE)$(DEFAULT)"
	@make --silent -C $(LIBFTDIR) && echo "$(GOTO_B)$(GREEN)Sucessfully created $(ITALIC)$(LIBFTFILE)$(DEFAULT)"

$(GNLDIR)/$(GNLFILE):
	@echo "Creating the library $(ITALIC)$(GNLFILE)$(DEFAULT)"
	@make --silent -C $(GNLDIR) && echo "$(GOTO_B)$(GREEN)Sucessfully created $(ITALIC)$(GNLFILE)$(DEFAULT)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	@echo "$(ORANGE)Compiling $(BOLD)$<$(DEFAULT)"
	@$(CC) $(CFLAGS) $(GDB) -I. -Iinc -c $< -o $@ && echo "$(GOTO_B)$(GREEN)Sucessfully compiled $(BOLD)$<$(DEFAULT)"

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	@echo "$(ORANGE)Compiling $(BOLD)$<$(DEFAULT)"
	@$(CC) $(CFLAGS) $(GDB) -I. -Iinc -c $< -o $@ && echo "$(GOTO_B)$(GREEN)Sucessfully compiled $(BOLD)$<$(DEFAULT)"

$(BUILDDIR):
	@mkdir -p $@ && echo "$(DIM)Directory $(ITALIC)$@/$(NOITA) created$(DEFAULT)"

.PHONY: clean fclean re norm
clean:
	@make --silent -C $(GNLDIR) clean
	@make --silent -C $(LIBFTDIR) clean
	@rm -f $(OBJ) $(MANDATORY_OBJ) $(BONUS_OBJ) && rm -rf $(BUILDDIR)\
		&& echo "Removed $(NAME)/$(NAME_BONUS)'s object files and their directory"


fclean: clean
	@make --silent -C $(GNLDIR) fclean
	@make --silent -C $(LIBFTDIR) fclean
	@rm -f $(NAME) $(NAME_BONUS) && echo "Removed $(BOLD)$(NAME)$(BOM) and $(BOLD)$(NAME_BONUS)"


re: fclean all

norm:
	@echo "$(CYAN)libft :$(DEFAULT)"
	@make --silent -C $(LIBFTDIR) norm
	@echo "$(CYAN)gnl :$(DEFAULT)"
	@make --silent -C $(GNLDIR) norm
	@echo "$(CYAN)pipex :$(DEFAULT)"
	@norminette -R CheckForbiddenSourceHeader $(SRC) $(MANDATORY) $(BONUS)
	@norminette -R CheckDefine inc/
