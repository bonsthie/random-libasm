NAME=libasm_tester

ARCH ?= x86
LIBASM_PATH ?= ../libasm.a
OBJ_DIR=obj
SRC=$(shell find base $(ARCH) -name '*.c')
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

$(info src = $(SRC))
$(info obj = $(OBJ))

CC = clang
CFLAG=-Wall -Wextra -Werror -O0 -g
INC=-I include

all : run

run: $(NAME)
	./$(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBASM_PATH)

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAG) $(INC) $< -o $@ -c

clean:
	rm -rf $(OBJ_DIR)


fclean: clean
	rm $(NAME)

re: fclean all run
