#include <stdio.h>
struct cpu_feat {
    unsigned char sse:1;
    unsigned char avx2:1;
    unsigned char avx512f:1;
};

extern struct cpu_feat __cpu_info ;

int main() {
	if (__cpu_info.sse)
		printf("crampte has sse\n");
}
