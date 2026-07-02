#pragma once
#include "Types.h"

static __forceinline constexpr u16 SizeFromRange( u16 start, u16 end ) {
	return ( end - start ) + 1;
}

static __forceinline constexpr u8 GetWordLowerByte( u16 value ) {
	return static_cast<u8>( value & 0xFFi16 );
}

static __forceinline constexpr u8 GetWordUpperByte( u16 value ) {
	return static_cast<u8>( value >> 8i16 );
}

static __forceinline constexpr u16 CreateWordFromBytes( u8 lower, u8 upper ) {
	return ( lower << 8i8 ) | upper;
}

static __forceinline u8 SetBitTo( u8& number, u8 n, bool x ) {
	number = ( number & ~( 1i8 << n ) ) | ( static_cast<u8>( x ) << n );
}

static __forceinline constexpr bool BitIsSet( u8 bit, u8 value ) {
	return ( value & ( 1i8 << bit ) ) != 0i8;
}