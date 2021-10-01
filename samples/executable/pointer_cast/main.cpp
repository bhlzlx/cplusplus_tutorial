#include <cstdio>
#include <cstdint>

class Base1 {
private:
public:
	virtual uint32_t getInt1() = 0;
};

class Base2 {
private:
public:
	virtual uint32_t getInt2() = 0;
};

class Inherited : public Base1, public Base2 {
private:
	uint32_t m_int1;
	uint32_t m_int2;
public:
	Inherited()
		: m_int1(0)
		, m_int2(1)
	{
	}

	virtual uint32_t getInt1() override {
		return m_int1;
	}

	virtual uint32_t getInt2() override {
		return m_int2;
	}
};

// static_cast
// dynamic_cast
// const_cast
// reinterpret_cast


int main() {
	Inherited* ptr = new Inherited();
	Base1* ptr1 = static_cast<Base1*>(ptr);
	Base2* ptr2 = static_cast<Base2*>(ptr);
	int a = ptr1->getInt1();
	int b = ptr2->getInt2();
	int c = ptr->getInt2();
	//
	Inherited* oriPtr = dynamic_cast<Inherited*>(ptr2);
	return 0;
}