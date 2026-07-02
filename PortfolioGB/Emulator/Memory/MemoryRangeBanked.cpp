#include "Types.h"
#include "MemoryRangeBanked.h"
#include <malloc.h>
#include <cassert>

MemoryRangeBanked::MemoryRangeBanked( u16 bankSize, u16 bankCount ) {
	u32 regionSize = bankCount * bankCount;
	memory = static_cast<u8*>( malloc( regionSize ) );

	this->bankCount = bankCount;
	this->bankSize = bankCount;
	size = regionSize;
	ownsMemory = true;
}

MemoryRangeBanked::MemoryRangeBanked( u8* memory, u16 bankSize, u16 bankCount ) {
	u32 regionSize = bankCount * bankSize;
	this->memory = memory;

	this->bankCount = bankCount;
	this->bankSize = bankSize;
	size = regionSize;
	ownsMemory = false;
}

MemoryRangeBanked::~MemoryRangeBanked( void ) {
	if(ownsMemory) free( memory );
}

u16 MemoryRangeBanked::GetSize( void ) const {
	return bankSize;
}

u32 MemoryRangeBanked::GetTrueSize( void ) const {
	return size;
}

u16 MemoryRangeBanked::LookupAddress( u16 address ) const {
	assert( address > 0 && address < bankSize );
	return ( currentBankAddress * bankSize ) + address;
}

u16 MemoryRangeBanked::GetBankIndex() const {
	return currentBankAddress;
}

void MemoryRangeBanked::SetBankIndex( u16 bankIndex ) {
	assert( bankIndex >= 0 && bankIndex < bankCount );
	currentBankAddress = bankIndex;
}

u8 MemoryRangeBanked::LoadByte( u16 address ) const {
	assert( address >= 0 && address < bankSize );
	return *( memory + address );
}

void MemoryRangeBanked::StoreByte( u16 address, u8 data ) {
	assert( address >= 0 && address < bankSize );
	*( memory + address ) = data;
}