NAME=libasm.a

ARCH=x86

SRC_DIR=$(ARCH)/src
OBJ_DIR=obj/$(ARCH)
INC_DIR=$(ARCH)/include
TEST_DIR=test

SRC=$(shell find $(SRC_DIR) -name '*.s')
OBJ = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC))

AS=nasm
AS_FLAG=-f elf64 -DVERBOSE

all : $(NAME)

$(NAME): $(OBJ)
	ar rcs -o $@ $(OBJ) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(AS) $(AS_FLAG) -I $(INC_DIR) $< -o $@

clean:
	rm -rf $(OBJ_DIR)


fclean: clean
	rm $(NAME)

re: fclean all

test: $(NAME)
	make -C $(TEST_DIR) -s


.PHONY: all clean fclean re
