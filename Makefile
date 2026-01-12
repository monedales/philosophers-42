# Program name
NAME = philo
# BONUS_NAME = 

# COLORS
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
YELLOW = \033[0;33m
LILAC = \033[0;95m
# 24-bit "truecolor" lilac (pastel). Terminal must support 24-bit color to show this.
LILAC_TRUE = \033[38;2;200;162;200m
RESET = \033[0m

# Directories
SRC_DIR = src
SRC_BONUS_DIR = src_bonus

INCLUDE_DIR = include


OBJ_DIR = obj
OBJ_BONUS_DIR = obj_bonus

#flag pro mac -Wno-deprecated-non-prototype -std=c17
# Compiler and base flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(INCLUDE_DIR) -pthread
AR = ar rcs
RM = rm -rf

# Detect system (macOS or Linux)
# UNAME_S := $(shell uname -s)


# Source files
SRC := philosophers.c \
       inits.c \
       routine.c \
       actions.c \
       monitor.c \
       utils.c \
       parsing.c \
       cleanup.c

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Bonus source files - only checker-specific files
SRC_BONUS = \

# Shared files needed for checker (reuse from src/)
SRC_SHARED = \
	
OBJS_BONUS = $(addprefix $(OBJ_BONUS_DIR)/, $(SRC_BONUS:.c=.o))
OBJS_SHARED = $(addprefix $(OBJ_BONUS_DIR)/, $(SRC_SHARED:.c=.o))

# Libraries


# Default rule
all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@$(MAKE) banner

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@


# Banner rules
banner:
	@echo ""
	@printf '%b\n' "$(LILAC_TRUE) ██████╗ ██╗  ██╗██╗██╗      ██████╗ ███████╗ ██████╗ ██████╗ ██╗  ██╗███████╗██████╗ ███████╗$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██╔══██╗██║  ██║██║██║     ██╔═══██╗██╔════╝██╔═══██╗██╔══██╗██║  ██║██╔════╝██╔══██╗██╔════╝$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██████╔╝███████║██║██║     ██║   ██║███████╗██║   ██║██████╔╝███████║█████╗  ██████╔╝███████╗$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██╔═══╝ ██╔══██║██║██║     ██║   ██║╚════██║██║   ██║██╔═══╝ ██╔══██║██╔══╝  ██╔══██╗╚════██║$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ██║     ██║  ██║██║███████╗╚██████╔╝███████║╚██████╔╝██║     ██║  ██║███████╗██║  ██║███████║$(RESET)"
	@printf '%b\n' "$(LILAC_TRUE) ╚═╝     ╚═╝  ╚═╝╚═╝╚══════╝ ╚═════╝ ╚══════╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝$(RESET)"
	@echo ""
	@echo "           🍝 Think → Eat → Sleep 🍝"
	@echo "            🍴    🍷    🍴" 
	@echo ""
	@echo "$(GREEN)Compiled successfully! Ready to philosophize!$(RESET)"
	@echo ""



.PHONY: all clean fclean re normi banner bonus

clean:
	@$(RM) $(OBJ_DIR) $(OBJ_BONUS_DIR)
	@echo "$(RED) $(NAME) objects removed$(RESET)"

fclean: clean
	@$(RM) $(NAME) $(BONUS_NAME)
	@echo "$(RED) $(NAME) deleted$(RESET)"

re: fclean all
	@echo "$(LILAC_TRUE)🔄 $(NAME) rebuild$(RESET)"

normi:
	@norminette -R CheckForbiddenSourceHeader $(addprefix $(SRC_DIR)/, $(SRC)) $(INCLUDE_DIR)/*.h
# Valgrind testing
# valgrind: $(NAME)

# Valgrind testing
valgrind: $(NAME)
	@echo "$(YELLOW)Running valgrind checking for data races...$(RESET)"
	@valgrind --tool=helgrind ./$(NAME) 5 800 200 200

valgrind-leak: $(NAME)
	@echo "$(YELLOW)Running valgrind checking for memory leaks...$(RESET)"
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 5 800 200 200