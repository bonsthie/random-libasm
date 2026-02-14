CC ?= clang
CFLAGS ?= -O3 -march=native -g
CFLAGS += -std=gnu11 -Wall -Wextra -fno-builtin

BENCH_CFLAGS = $(CFLAGS) -D_GNU_SOURCE

LIBASM = ../libasm.a

BENCHES = strlen_bench strcpy_bench

all: $(BENCHES)

strlen_bench.o: strlen_bench.c
	$(CC) $(BENCH_CFLAGS) -c $< -o $@

strlen_bench: strlen_bench.o $(LIBASM)
	$(CC) $(LDFLAGS) $^ -o $@

strcpy_bench.o: strcpy_bench.c
	$(CC) $(BENCH_CFLAGS) -c $< -o $@

strcpy_bench: strcpy_bench.o $(LIBASM)
	$(CC) $(LDFLAGS) $^ -o $@

run: strlen_bench
	./strlen_bench

run_strcpy: strcpy_bench
	./strcpy_bench

clean:
	rm -f *.o $(BENCHES)

.PHONY: all run run_strcpy clean
