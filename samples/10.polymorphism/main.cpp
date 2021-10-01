#include <cstdio>
#include <cstdint>
#include <cstdlib>

class ISpeaker{
protected:
	size_t b;
public:
	ISpeaker( size_t _v )
		: b(_v) 
	{}
	virtual void speak() = 0;
};

class Dog : public ISpeaker {
public:
	Dog()
		: ISpeaker(0)
	{}
	//
	virtual void speak() override {
		printf("woof! %llu\n", b);
	}
};

class Human : public ISpeaker {
private:
	size_t c;
public:
	Human()
		: ISpeaker(1)
		, c(2)
	{}

	virtual void speak() override {
		printf("hello! %llu\n", c);
	}
};

int main( int argc, char** _argv ) {
	Human* pHuman = new Human();
	Dog* pDog = new Dog();
	//
	ISpeaker* speaker1 = (ISpeaker*)pHuman;
	ISpeaker* speaker2 = (ISpeaker*)pDog;
	//
	speaker2->speak();
	speaker1->speak();
	//
	return 0;
}