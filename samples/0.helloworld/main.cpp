#include <cstdio>
#include <cstdint>


class Stu {
private:
	uint32_t m_age;
	uint32_t m_id;
public:
	Stu( uint32_t _age, uint32_t _id )
		: m_age( _age )
		, m_id( _id )
	{
	}
	Stu(Stu&& _stu) {
		m_age = _stu.m_age;
		m_id = _stu.m_id;
	}
	uint32_t getAge() {
		return m_age;
	}
	~Stu() {
		printf("dtor");
	}
};

Stu createStudent( uint32_t _age, uint32_t _id ) {
	return { _age, _id };
}

int plus(int _x, int _y) {
	return _x + _y
}

int main() {
	plus(2, 3) = 4;
	//return stu.getAge();
}