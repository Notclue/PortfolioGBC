#pragma once
#include "Types.h"
#include "Emulator/Bitmath.h"

#define DEFINE_REGION(name, start, end) const u16 name##_START = start; \
const u16 name##_END = end; \
const u16 name##_SIZE = SizeFromRange( start, end )

DEFINE_REGION( ROM, 0x0000, 0x7FFF );
DEFINE_REGION( VRAM, 0x8000, 0x9FFF );
DEFINE_REGION( SRAM, 0xA000, 0xBFFF );

DEFINE_REGION( WRAM1, 0xC000, 0xCFFF );
DEFINE_REGION( WRAM2, 0xD000, 0xDFFF );
DEFINE_REGION( WRAM, WRAM1_START, WRAM2_END );

DEFINE_REGION( ECHO, 0xE000, 0xFDFF );
DEFINE_REGION( OAM, 0xFE00, 0xFE9F );

DEFINE_REGION( GARBAGE, 0xFEA0, 0xFEFF );
DEFINE_REGION( REGISTERS, 0xFF00, 0xFE7F );
DEFINE_REGION( HRAM, 0xFE80, 0xFFFE );
DEFINE_REGION( SPECIAL, REGISTERS_START, 0xFFFF );


class IMemory {
public:
	//The amount of memory that the system can access at once, allowing for swappable memory banks.
	virtual u16 GetSize( void ) const = 0;

	//Total amount of real memory this class owns.
	virtual u32 GetTrueSize( void ) const = 0;

	virtual u8 LoadByte( u16 address ) const = 0;
	virtual void StoreByte( u16 address, u8 data ) = 0;
	
	/*While there is undoubtably some kind of hack to allow reading a word directly, it may be possible that
	a 16-bit memory access crosses the boundary between different regions of memory that have different behavior.*/

	short LoadWord( u16 address ) const {
		return CreateWordFromBytes(
			LoadByte( address ),
			LoadByte( address + 1 ) );
	}
	
	void StoreWord( u16 address, u16 data ) {
		StoreByte( address, GetWordLowerByte( data ) );
		StoreByte( address + 1, GetWordUpperByte( data ) );
	}
};