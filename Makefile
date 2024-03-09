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
				input_output.c	\
				pipex.c			\
				utils.c			\

MAIN_FILE	:=	main.c

SRC_PATH	:=	src
SRC		:=	$(MAIN_FILE)	\
			$(addprefix $(SRC_PATH)/,$(FILES))

OBJ_PATH	:=	build
OBJ		:=	$(addprefix $(OBJ_PATH)/,$(FILES:.c=.o))	\
			$(addprefix $(OBJ_PATH)/,$(MAIN_FILE:.c=.o))

CC		:=	cc
CFLAGS	:=	-Wall -Wextra -Werror
GDB		:=
export GDB

INC_PATH	:=	inc
CINC		:=	-I. -I$(INC_PATH)

FT_PATH	:=	libft
FT		:=	ft
FT_FILE	:=	lib$(FT).a

LDLIBS	:=	-l$(FT)
LDFLAGS	:=	-L$(FT_PATH)

NAME		:=	pipex

.PHONY: all
all: $(NAME)

$(NAME): $(FT_PATH)/$(FT_FILE) $(OBJ)
	@echo "$(ORANGE)$(ITA)Linking$(NOITA) into $(BOLD)$@$(DEFAULT)..."
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS) \
		&& echo "$(GOTO_B)$(GREEN)Successfully $(ITA)linked$(NOITA) into $(BOLD)$@$(DEFAULT)"


$(FT_PATH)/$(FT_FILE):
	@echo "$(ORANGE)$(ITA)Creating$(NOITA) $(BOLD)$@$(DEFAULT)..."
	@make --silent -C $(FT_PATH) > /dev/null \
		&& echo "$(GOTO_B)$(GREEN)Successfully $(ITA)created$(NOITA) $(BOLD)$@$(DEFAULT)"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	@echo "$(ORANGE)$(ITA)Compiling$(NOITA) $(BOLD)$<$(DEFAULT)..."
	@$(CC) $(CFLAGS) $(GDB) $(CINC) -c $< -o $@ \
		&& echo "$(GOTO_B)$(GREEN)Successfully $(ITA)compiled$(NOITA) $(BOLD)$<$(DEFAULT)"

$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
	@echo "$(ORANGE)$(ITA)Compiling$(NOITA) $(BOLD)$<$(DEFAULT)..."
	@$(CC) $(CFLAGS) $(GDB) $(CINC) -c $< -o $@ \
		&& echo "$(GOTO_B)$(GREEN)Successfully $(ITA)compiled$(NOITA) $(BOLD)$<$(DEFAULT)"

$(OBJ_PATH):
	@( mkdir $@ 2> /dev/null \
		&& echo "$(DIM)Successfully $(ITA)created$(NOITA) directory $(DEFAULT)$@/" ) \
		|| :

.PHONY: clean fclean re
clean:
	@( rm -r $(OBJ_PATH) 2> /dev/null \
		&& echo "$(PURPLE)$(ITA)Removed$(NOITA) object files and their directory$(DEFAULT)" ) \
		|| :

fclean: clean
	@( rm $(NAME) 2> /dev/null \
		&& echo "$(PURPLE)$(ITA)Removed$(NOITA) executable $(NAME)$(DEFAULT)" ) \
		|| :

re: fclean all

.PHONY: cleanlib

cleanlib: $(FT_PATH)
	@echo "Cleaning the library..."
	@make --silent fclean -C $(FT_PATH) \
		&& echo "$(GOTO_B)$(GREEN)Successfully $(ITA)cleaned$(NOITA) the library$(DEFAULT)"
