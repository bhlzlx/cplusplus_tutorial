#include <cstdio>
#include <cstdint>
#include <vector>
#include <string>


std::string add( const char* n1, const char* n2 ) {
	std::string rst;
	constexpr char chZero = '0';
	constexpr char chOne = '1';
	constexpr char chNine = '9';
	const uint32_t n1Length = (uint32_t)strlen(n1);
	const uint32_t n2Length = (uint32_t)strlen(n2);
	uint32_t length = n1Length > n2Length ? n1Length : n2Length;
	n1+=n1Length-1;
	n2+=n2Length-1;
	rst.resize(length);
	auto iter = rst.rbegin();
	auto end = rst.rend();
	bool overflow = false;
	while( iter!=end ) {
		char chN1 = *n1;
		char chN2 = *n2;
		if(chN1) {
			--n1;
		} else {
			chN1 = chZero;
		}
		if(chN2) {
			--n2;
		} else {
			chN2 = chZero;
		}
		// ========================= 
		char n = chN1 + chN2 - chZero;
		if(overflow) {
			n+=1;
			overflow = false;
		}
		if(n<=chNine) {
			*iter = n;
		} else {
			*iter = n-10;
			overflow = true;
		}
		++iter;
	}
	if(overflow) {
		rst.resize(rst.size()+1);
		auto iter = rst.rbegin();
		auto last = rst.rend() - 1;
		while(iter!=last) {
			auto next = iter+1;
			*iter = *next;
			iter = next;
		}
		rst[0] = chOne;
	}
	return rst;
}


int main() {
	std::string rst = add( "5465666665555555554444", "5465666665555555554444" );
	printf("%s\n", rst.c_str());
	return 0;
}