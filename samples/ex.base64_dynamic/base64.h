#pragma once
#include <cstdint>
#include <string>

#ifdef BASE64_SHARED
#define BASE64_EXPORT __declspec(dllexport)
#else
#define BASE64_EXPORT __declspec(dllimport)
#endif

namespace base64
{
	BASE64_EXPORT void Base64Encode(const int8_t * _data, size_t _size, char* _base64);
	BASE64_EXPORT size_t Base64Decode(const char * _base64, size_t _length, int8_t * _data, size_t _size);
}