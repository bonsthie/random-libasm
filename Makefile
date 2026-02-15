NAME=libasm.a
LIBASM_PATH=$(shell realpath $(NAME))

ARCH=x86

SRC_DIR=$(ARCH)/src
OBJ_DIR=obj/$(ARCH)
INC_DIR=$(ARCH)/include
TEST_DIR=tester

SRC=$(shell find $(SRC_DIR) -name '*.s')
OBJ = $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC))

AS=nasm

AS_FLAGS=-f elf64

ifeq ($(ENABLE_IFUNC), true)
	AS_FLAGS += -D__IFUNC_ENABLE
endif

ifeq ($(VERBOSE), true)
	AS_FLAGS += -DVERBOSE
endif

all : $(NAME)

$(NAME): $(OBJ)
	ar rcs -o $@ $(OBJ) 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s
	mkdir -p $(@D)
	$(AS) $(AS_FLAGS) -I $(INC_DIR) $< -o $@

clean:
	rm -rf $(OBJ_DIR)


fclean: clean
	rm $(NAME)

re: fclean all

test: $(NAME)
	@make -C $(TEST_DIR) -s ARGS=$(ARGS) \
							LIBASM_PATH=$(LIBASM_PATH)

.PHONY: all clean fclean re
