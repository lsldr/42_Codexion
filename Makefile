NAME    :=  codexion

CC      :=  cc
CFLAGS  :=  -Wall -Wextra -Werror -pthread
CPPFLAGS:=  -Isrc

SRC_DIR :=  src
OBJ_DIR :=  obj

SRC     := main.c \
		   	$(SRC_DIR)/c_routine.c		\
		   	$(SRC_DIR)/check_uint.c 	\
			$(SRC_DIR)/clean_all.c		\
		   	$(SRC_DIR)/codexion.c		\
			$(SRC_DIR)/dongle_ops.c		\
			$(SRC_DIR)/init_ds.c		\
			$(SRC_DIR)/init_threads.c	\
			$(SRC_DIR)/m_routine.c		\
           	$(SRC_DIR)/parse_args.c 	\
			$(SRC_DIR)/queue_ops.c		\
			$(SRC_DIR)/queue_utils.c	\
			$(SRC_DIR)/strto_num.c		\
			$(SRC_DIR)/utils.c			\

OBJ     := $(SRC:%.c=$(OBJ_DIR)/%.o)

HEADER  := $(SRC_DIR)/codexion.h


.PHONY: all clean fclean re asan tsan valgrind


all: $(NAME)

$(NAME): $(OBJ)
		@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
		@echo "Built the codexion binary"

$(OBJ_DIR)/%.o: %.c $(HEADER)
		@mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@
		@printf 'Compiled %s\n' "$(@F)"

clean:
		@rm -rf $(OBJ_DIR)

fclean: clean
		@rm -f $(NAME)

asan: fclean
	@$(MAKE) all CFLAGS="$(CFLAGS) -fsanitize=address -g"
	@printf '\nAddressSanitizer is on\n'

tsan: fclean
	@$(MAKE) all CFLAGS="$(CFLAGS) -fsanitize=thread -g"
	@printf '\nThreadSanitizer is on\n'

valgrind: fclean
	@$(MAKE) all CFLAGS="$(CFLAGS) -g"
	@printf '\n-g flag applied\n'

re: fclean all
