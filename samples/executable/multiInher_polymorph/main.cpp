#include <cstdio>
#include <cstdint>
#include <cstdlib>

class IRunner {
private:
	size_t a;
public:
	IRunner()
		: a(0){
	}
	virtual void run() = 0;
};

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
		: ISpeaker(1)
	{}
	//
	virtual void speak() override {
		printf("woof! %llu\n", b);
	}
};

class RunnerDog : public IRunner, public Dog {
public:
	RunnerDog()
	{}

	virtual void run() override {
		printf("run with 4 legs\n");
	}
};

int main( int argc, char** _argv ) {
	RunnerDog* pDog = new RunnerDog();
	Dog* simpleDog = new Dog();
	pDog->speak();
	{ // 等价于
		ISpeaker* speaker1 = static_cast<ISpeaker*>(pDog);
		speaker1->speak();
	}

	ISpeaker* speaker = static_cast<ISpeaker*>( simpleDog );	
	RunnerDog* runnerDog = dynamic_cast<RunnerDog*>(speaker);
	// RTTI 信息
	if(runnerDog){
		runnerDog->run();
	}
	//
	// 子类 -> 基类 （ static_cast<>() ）
	// 基类 -> 子类 （ dynamic_cast<>() ）
	// 有可能变化
	// RunnerDog* runnerDog = (RunnerDog*)(speaker);
 	return 0;
}