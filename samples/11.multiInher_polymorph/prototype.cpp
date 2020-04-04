#include <cstdio>
#include <cstdlib>
#include <cstdint>

extern "C" {

	#define RTTI_INFORMATION

	struct RunnerTable {
		RTTI_INFORMATION
		void(* run)(void* _ptr);
	};

	struct SpeakerTable {
		RTTI_INFORMATION
		void(* speak )( void* _ptr );
	};

	void __dog_run( void* _ptr ) {
		printf("run with 4 legs");
	}

	void __dog_speak( void* _ptr ) {
		uint8_t* p = (uint8_t*)_ptr;
		p+=sizeof(SpeakerTable*);
		size_t b = *((size_t*)p);
		printf("woof! %llu\n", b);
	}

	const static RunnerTable __dogRunnerTable = {
		RTTI_INFORMATION
		__dog_run
	};

	const static SpeakerTable __dogSpeakTable = {
		RTTI_INFORMATION
		__dog_speak
	};

	struct __dog {
		const SpeakerTable* vt;
		size_t b;
	};

	struct __runner_dog {
		const RunnerTable* vt1;
		size_t a;
		const SpeakerTable* vt2;
		size_t b;
	};

	__dog * createDog() {
		__dog* ptr = (__dog*)malloc(sizeof(__dog));
		ptr->vt = &__dogSpeakTable;
		ptr->b = 0;
		return ptr;
	}

	__runner_dog* createRunnerDog() {
		__runner_dog* ptr = (__runner_dog*)malloc(sizeof(__runner_dog));
		ptr->vt1 = &__dogRunnerTable;
		ptr->a = 0;
		ptr->vt2 = &__dogSpeakTable;
		ptr->b = 1;
		return ptr;
	}

};

int main( int _argc, char** _argv ) {
    __dog* dog = createDog();
	__runner_dog* runnerDog = createRunnerDog();

	SpeakerTable** speaker = nullptr;{
		uint8_t* ptr = (uint8_t*)runnerDog;
		union {
			const SpeakerTable* __runner_dog::* memOffset;
			size_t offset;
		} u;
		u.memOffset = &__runner_dog::vt2;
		ptr += u.offset;
		speaker = (SpeakerTable**)ptr;
	}
	(*speaker)->speak(speaker);
	// 等价于
	runnerDog->vt2->speak(speaker);
	// 但不等价于
	runnerDog->vt2->speak(runnerDog); // 这是错误的
	//
	return 0;
}