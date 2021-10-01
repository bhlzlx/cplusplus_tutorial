#pragma once
#include <cstdint>
#include <string>

namespace base64
{
	void Base64Encode(const int8_t * _data, size_t _size, char* _base64);
	size_t Base64Decode(const char * _base64, size_t _length, int8_t * _data, size_t _size);
}