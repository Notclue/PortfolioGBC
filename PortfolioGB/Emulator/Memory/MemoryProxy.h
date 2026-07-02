#pragma once
#include "IMemory.h"
#include "Types.h"
#include <vector>

struct BlockedRegion_t {
	u16 startIndex;
	u16 endIndex;
};

class MemoryProxy : public IMemory {
private:
	std::vector<BlockedRegion_t> blockedRegions;
	IMemory* target;

public:
	MemoryProxy( IMemory& target );

	u16 GetSize( void ) const;
	u32 GetTrueSize( void ) const;

	u8 LoadByte( u16 address ) const;
	void StoreByte( u16 address, u8 data );

	void BlockRegion( u16 startIndex, u16 endIndex );
	void UnblockRegion( u16 startIndex, u16 endIndex );
	size_t FindRegionIndex( u16 startIndex, u16 endIndex ) const;

	bool AddressIsBlocked( u16 address ) const;
};

