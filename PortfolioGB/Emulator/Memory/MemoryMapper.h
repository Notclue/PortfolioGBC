#pragma once
#include "IMemory.h"
#include "Types.h"
#include <initializer_list>
#include <vector>

struct MapperRegion_t {
	IMemory* region;
	u16 startIndex;
};

class MemoryMapper : public IMemory {
private:
	std::vector<MapperRegion_t> regions;
	std::vector<u16> lookupTable;

	u16 size;
	u32 trueSize;

	u16 lookupFactor;

	u16 CalculateLookupFactor( void ) const;
	void BuildLookupTable( void );

public:
	MemoryMapper( std::initializer_list<IMemory*> list, u16 overrideSize = -1 );

	u16 GetSize( void ) const;
	u32 GetTrueSize( void ) const;

	u8 LoadByte( u16 address ) const;
	void StoreByte( u16 address, u8 data );
};