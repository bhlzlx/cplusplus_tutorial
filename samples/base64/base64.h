#pragma once
#include <cstdint>
#include <string>

namespace base64
{
	void Base64Encode(const uint8_t * _data, size_t _size, std::string& _base64);
	size_t Base64Decode(const char * _base64, size_t _length, uint8_t * _data, size_t _size);
}


