#include <cstdio>
#include <cstdint>

int main( int argc, char** _argv ) {
	printf("hello,main!");
	return 0;
}

extern "C" {
	int YetAnotherEntry(  /*int argc, char** _argv*/ ) {
		printf("Yet another entry!");
		return 0;	
	}
}