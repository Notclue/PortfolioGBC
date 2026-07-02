#include "MemoryRange.h"
#include "Types.h"
#include <cassert>
#include <malloc.h>

MemoryRange::MemoryRange( u16 size ) {
	memory = static_cast<u8*>( malloc( size ) );
	this->size = size;
	ownsMemory = true;
}

MemoryRange::MemoryRange( u8* memory, u16 size ) {
	this->memory = memory;
	this->size = size;
	ownsMemory = false;
}

MemoryRange::~MemoryRange( void ) {
	if( ownsMemory ) free( memory );
}

u16 MemoryRange::GetSize( void ) const {
	return static_cast<u16>( size );
}

u32 MemoryRange::GetTrueSize( void ) const {
	return size;
}

u8 MemoryRange::LoadByte( u16 address ) const {
	assert( address >= 0 && address < GetSize() );
	return *( memory + address );
}

void MemoryRange::StoreByte( u16 address, u8 data ) {
	assert( address >= 0 && address < GetSize() );
	*( memory + address ) = data;
}