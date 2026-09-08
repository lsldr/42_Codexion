NAME    :=  codexion

CC      :=  cc
CFLAGS  :=  -Wall -Wextra -Werror -pthread
CPPFLAGS:=  -Isrc


SRC_DIR :=  src
OBJ_DIR :=  obj

SRC     := main.c \
            $(SRC_DIR)/parse_argv.c \
            $(SRC_DIR)/is_uint.c

OBJ     := $(SRC:%.c=$(OBJ_DIR)/%.o)

HEADER  := $(SRC_DIR)/codexion.h


.PHONY: all clean fclean re


all: $(NAME)

$(NAME): $(OBJ)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: %.c $(HEADER)
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@


clean:
		rm -rf $(OBJ_DIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

