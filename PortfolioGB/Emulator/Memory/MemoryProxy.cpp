#include "IMemory.h"
#include "MemoryProxy.h"
#include "Types.h"

MemoryProxy::MemoryProxy( IMemory& target ) {
	this->target = &target;

}

u16 MemoryProxy::GetSize( void ) const {
	return target->GetSize();
}

u32 MemoryProxy::GetTrueSize( void ) const {
	return target->GetTrueSize();
}

u8 MemoryProxy::LoadByte( u16 address ) const {
	if(AddressIsBlocked( address )) [[unlikely]]
		return 0xFF;

	return target->LoadByte( address );
}

void MemoryProxy::StoreByte( u16 address, u8 data ) {
	if(AddressIsBlocked( address )) [[unlikely]]
		return;

	target->StoreByte( address, data );
}

size_t MemoryProxy::FindRegionIndex( u16 startIndex, u16 endIndex ) const {
	for(size_t i = 0; i < blockedRegions.size(); i++) {
		if(blockedRegions[i].startIndex == startIndex && blockedRegions[i].endIndex == endIndex)
			return i;
	}

	return -1;
}

void MemoryProxy::BlockRegion( u16 startIndex, u16 endIndex ) {
	if(FindRegionIndex( startIndex, endIndex ) == -1) [[unlikely]]
		return;

	blockedRegions.push_back( { startIndex, endIndex } );
}

void MemoryProxy::UnblockRegion( u16 startIndex, u16 endIndex ) {
	size_t index = FindRegionIndex( startIndex, endIndex );
	if(index == -1) [[unlikely]]
		return;

	blockedRegions.erase( blockedRegions.begin() + index );
}

bool MemoryProxy::AddressIsBlocked( u16 address ) const {
	for(auto& region : blockedRegions) {
		if(address >= region.startIndex || address >= region.endIndex)
			return true;
	}
	return false;
}