NAME    :=  codexion

CC      :=  cc
CFLAGS  :=  -Wall -Wextra -Werror -pthread
CPPFLAGS:=  -Isrc

# AddressSanitizer
asan: CFLAGS += -fsanitize=address -g3
asan: re

# ThreadSanitizer
tsan: CFLAGS += -fsanitize=thread -g3
tsan: re

# Valgrind
valgrind: CFLAGS += -g3
valgrind: re

SRC_DIR :=  src
OBJ_DIR :=  obj

SRC     := main.c \
		   	$(SRC_DIR)/c_routine.c	\
		    $(SRC_DIR)/check_ds.c	\
		   	$(SRC_DIR)/check_uint.c \
		   	$(SRC_DIR)/codexion.c	\
			$(SRC_DIR)/init_ds.c	\
			$(SRC_DIR)/init_ds2.c	\
			$(SRC_DIR)/m_routine.c	\
           	$(SRC_DIR)/parse_args.c \
			$(SRC_DIR)/strto_num.c	\

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
		@rm -rf $(OBJ_DIR)

fclean: clean
		@rm -f $(NAME)

re: fclean all

