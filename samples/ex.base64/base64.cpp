#include "base64.h"
#include <cassert>

namespace base64
{
	const static int8_t EncTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const static int8_t DecTable[] = {
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
		-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
		-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
	};
	//
	void Base64Encode(const int8_t * _data, size_t _size, char * _base64)
	{
		//
		char cache[5] = { 0 };
		size_t fullRound = _size / 3;
		size_t fullRoundLeft = _size % 3;
		size_t round = 0;
		for (; round < fullRound; ++round) {
			cache[0] = EncTable[_data[round*3] >> 2];
			cache[1] = EncTable[((_data[round * 3] & 0b11) << 4) | (_data[round * 3 + 1] >> 4)];
			cache[2] = EncTable[((_data[round * 3 + 1] & 0b1111) << 2) | (_data[round * 3 + 2] >> 6)];
			cache[3] = EncTable[_data[round * 3 + 2] & 0x3f];
			memcpy( _base64, cache, 4);
			_base64 = _base64 + 4;
		}
		if (fullRoundLeft) {
			switch (fullRoundLeft)
			{
				case 1: {
					cache[0] = EncTable[_data[round * 3] >> 2];
					cache[1] = EncTable[((_data[round * 3] & 0b11) << 4)];
					cache[2] = cache[3] = '=';
					break;
				}
				case 2: {
					cache[0] = EncTable[_data[round * 3] >> 2];
					cache[1] = EncTable[((_data[round * 3] & 0b11) << 4) | (_data[round * 3 + 1] & 0b1111)];
					cache[2] = EncTable[((_data[round * 3 + 1] & 0b1111) << 2)];
					cache[3] = '=';
					break;
				}
			}
			memcpy( _base64, cache, 4);
			_base64 = _base64 + 4;
		}
		*_base64 = 0;
	}

	size_t Base64Decode(const char * _base64, size_t _length, int8_t * _data, size_t _size)
	{
		assert(_length >= 4);
		if (_size < (_length >> 2) / 3) {
			return 0;
		}
		size_t i = 0;
		int8_t * ptr = _data;
		if (_length > 4) {
			for (; i < _length - 4; i += 4) {
				*ptr = (DecTable[_base64[i]] << 2) | (DecTable[_base64[i + 1]] >> 4); ++ptr;
				*ptr = ((DecTable[_base64[i + 1]] & 0xf) << 4) | (DecTable[_base64[i + 2]] >> 2); ++ptr;
				*ptr = ((DecTable[_base64[i + 2]] & 0b11) << 6) | DecTable[_base64[i + 3]]; ++ptr;
			}
		}
		*ptr = (DecTable[_base64[i]] << 2) | (DecTable[_base64[i + 1]] >> 4); ++ptr;
		if (_base64[i + 2] == '=') {
			return ptr - _data;
		}			
		*ptr = ((DecTable[_base64[i + 1]] & 0xf) << 4) | (DecTable[_base64[i + 2]] >> 2); ++ptr;
		if (_base64[i + 3] == '=') {
			return ptr - _data;
		}			
		*ptr = ((DecTable[_base64[i + 2]] & 0b11) << 6) | DecTable[_base64[i + 3]]; ++ptr;
		return ptr - _data;
	}
}


