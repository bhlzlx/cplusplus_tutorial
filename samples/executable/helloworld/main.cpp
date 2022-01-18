#include <cstdio>
#include <array>
#include <vector>
#include <cstdint>
#include <random>
#include <time.h>
#include <cstdio>

int main() {
	srand(time(0));
	uint8_t i = rand() % 256;
	while(true) {
		uint32_t num;
		scanf("%d", &num);
		if(num == i) {
			printf("you got the anwser!");
			break;
		} else if(num > i) {
			printf("the answer is less than you input!\n");
		} else if(num < i) {
			printf("the answer is large than you input!\n");
		}
	}
	return 0;
}
