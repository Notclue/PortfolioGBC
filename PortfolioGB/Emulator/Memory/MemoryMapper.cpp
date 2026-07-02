#include "Emulator/Memory/MemoryRange.h"
#include "Emulator/Memory/MemoryRangeBanked.h"
#include "IMemory.h"
#include "MemoryMapper.h"
#include "Types.h"
#include <cassert>
#include <initializer_list>
#include <vector>

//TODO: move to somewhere else.
static int findGCD( u32 a, u32 b ) {
	int temp;
	while(b != 0) {
		temp = b;
		b = a % b;
		a = temp;
	}
	return a;
}

MemoryMapper::MemoryMapper( std::initializer_list<IMemory*> list, u16 overrideSize ) {
	assert( list.size() > 1 );

	for(auto& region : list) {
		regions.push_back( { region, 0 } );
	}
	for(size_t i = 1; i < regions.size(); i++) {
		regions[i].startIndex = regions[i].region->GetSize() + regions[i - 1].startIndex;
	}

	//calculate sizes
	size = 0;
	trueSize = 0;
	for(u32 i = 1; i < regions.size(); i++) {
		size += regions[i].region->GetSize();
		trueSize += regions[i].region->GetTrueSize();
	}

	if(overrideSize != -1) {
		assert( overrideSize > 0 && overrideSize < size );
		size = overrideSize;
	}

	BuildLookupTable();
}

void MemoryMapper::BuildLookupTable( void ) {
	lookupFactor = CalculateLookupFactor();

	lookupTable.reserve( size / lookupFactor );
	for(size_t r = 0; r < regions.size(); r++) {
		const MapperRegion_t& region = regions[r];

		size_t lookupStartIndex = region.startIndex / lookupFactor;
		size_t lookupEndIndex;
		if(r == regions.size() - 1) {
			lookupEndIndex = lookupTable.size() - 1;
		} else {
			lookupEndIndex = ( region.startIndex + (size_t)region.region->GetSize() - 1 ) / lookupFactor;
		}

		for(size_t i = lookupStartIndex; i < lookupEndIndex; i++) {
			lookupTable[i] = (u16)r;
		}
	}
}

u16 MemoryMapper::GetSize( void ) const {
	return size;
}

u32 MemoryMapper::GetTrueSize( void ) const {
	return trueSize;
}

u16 MemoryMapper::CalculateLookupFactor( void ) const {
	u16 result = regions[0].region->GetSize();
	for(u16 i = 1; i < regions.size(); i++) {
		result = findGCD( result, regions[i].region->GetSize() );
	}
	return result;
}

u8 MemoryMapper::LoadByte( u16 address ) const {
	u16 regionIndex = lookupTable[address / lookupFactor];

	const MapperRegion_t& region = regions[regionIndex];
	u16 localAddress = address - region.startIndex;

	return region.region->LoadByte( localAddress );
}

void MemoryMapper::StoreByte( u16 address, u8 data ) {
	u16 regionIndex = lookupTable[address / lookupFactor];

	const MapperRegion_t& region = regions[regionIndex];
	u16 localAddress = address - region.startIndex;

	region.region->StoreByte( localAddress, data );
}