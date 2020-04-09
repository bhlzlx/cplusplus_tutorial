#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <utility>

struct Test1
{
	int intval;
	uint8_t ss;
	uint16_t size;
};

template< class T1, class T2 >
struct ClazzMemberUnion {
	union {
		T1 v1;
		T2 v2;
	};
};

template < class C, class M, class OffsetType = M C::*, std::enable_if_t< sizeof(OffsetType) == 1U, bool > = true, class ReturnType =  uint8_t >
constexpr ReturnType member_offset_cast__( M C::* && _v ) {
	ClazzMemberUnion<OffsetType, ReturnType> u = { std::forward<M C::*>(_v) };
	return u.v2;
}

template < class C, class M, class OffsetType = M C::*, std::enable_if_t< sizeof(OffsetType) == 2U, bool > = true, class ReturnType =  uint16_t >
constexpr ReturnType member_offset_cast__( M C::* && _v ) {
	ClazzMemberUnion<OffsetType, ReturnType> u = { std::forward<M C::*>(_v) };
	return u.v2;
}

template < class C, class M, class OffsetType = M C::*, std::enable_if_t< sizeof(OffsetType) == 4U, bool > = true, class ReturnType =  uint32_t >
constexpr ReturnType member_offset_cast__( M C::* && _v ) {
	ClazzMemberUnion<OffsetType, ReturnType> u = { std::forward<M C::*>(_v) };
	return u.v2;
}

template < class C, class M, class OffsetType = M C::*, std::enable_if_t< sizeof(OffsetType) == 8U, bool > = true, class ReturnType =  uint64_t >
constexpr ReturnType member_offset_cast__( M C::* && _v ) {
	ClazzMemberUnion<OffsetType, ReturnType> u = { std::forward<M C::*>(_v) };
	return u.v2;
}

template < class T >
constexpr uint32_t member_offset_cast( T&& _v ) {
	return static_cast<uint32_t>( member_offset_cast__( std::forward<T>(_v) ) );
}

int main() {
	uint32_t o = member_offset_cast( &Test1::intval );
	return 0;
}