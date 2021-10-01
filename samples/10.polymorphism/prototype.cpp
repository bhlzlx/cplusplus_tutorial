#include <cstdio>
#include <cstdlib>
#include <cstdint>

extern "C" {

	struct SpeakerTable {
		void(* speak )( void* _ptr );
	};

	void __dog_speak( void* _ptr ) {
		uint8_t* p = (uint8_t*)_ptr;
		p+=sizeof(SpeakerTable*);
		size_t b = *((size_t*)p);
		printf("woof! %llu\n", b);
	}

	void __human_speak( void* _ptr ) {
		uint8_t* p = (uint8_t*)_ptr;
		p+=sizeof(SpeakerTable*);
		p+=sizeof(size_t);
		size_t b = *((size_t*)p);
		printf("hello! %llu\n", b);
	}

	const static SpeakerTable __dogSpeakTable = {
		__dog_speak
	};
	const static SpeakerTable __humanSpeakTable = {
		__human_speak
	};

	struct __ispeaker {
		const SpeakerTable* vt;
		size_t b;
	};

	struct __dog {
		const SpeakerTable* vt;
		size_t b;
	};

	struct __human {
		const SpeakerTable* vt;
		size_t b;
		size_t c;
	};

	__dog * createDog() {
		__dog* ptr = (__dog*)malloc(sizeof(__dog));
		ptr->vt = &__dogSpeakTable;
		ptr->b = 0;
		return ptr;
	}

	__human* createHuman() {
		__human* ptr = (__human*)malloc(sizeof(__human));
		ptr->vt = &__humanSpeakTable;
		ptr->b = 1;
		ptr->c = 2;
		return ptr;
	}

};

int main( int _argc, char** _argv ) {
    __dog* dog = createDog();
	__human* human = createHuman();
	//
	__ispeaker* speaker1 = (__ispeaker*)dog;
	__ispeaker* speaker2 = (__ispeaker*)human;
	//
	speaker1->vt->speak(speaker1);
	speaker2->vt->speak(speaker2);
	return 0;
}