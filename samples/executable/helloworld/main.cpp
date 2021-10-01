#include <cstdio>
#include <cstdint>

class CString {
private:
	size_t		_capacity;
public:
	CString() 
		: _capacity( 0 )
	{
		printf("con\n");
	}

	CString( const CString& str )
		: _capacity( str._capacity )
	{
		printf("copy\n");
	}

	CString& operator=(const CString& str) {
		printf("assign\n");
		this->_capacity = str._capacity;
		return *this;
	}

	~CString() {
		printf("con\n");
	}
};

CString createString() {
	CString str;
	return str;
}

int main() {
	CString str = createString();
	printf("hello,world!");
	return 0;
}