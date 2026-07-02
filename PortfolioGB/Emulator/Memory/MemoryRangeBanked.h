#pragma once
#include "Types.h"
#include "IMemory.h"

class MemoryRangeBanked : public IMemory {
private:
	u8* memory;
	u32 size;
	u16 bankCount;
	u16 bankSize;
	bool ownsMemory;

	u16 currentBankAddress = 0;

	u16 LookupAddress( u16 address ) const;

public:
	MemoryRangeBanked( u16 bankSize, u16 bankCount );
	MemoryRangeBanked( u8* memory, u16 bankSize, u16 bankCount );
	~MemoryRangeBanked( void );

	virtual u16 GetSize( void ) const;
	virtual u32 GetTrueSize( void ) const;

	virtual u8 LoadByte( u16 address ) const;
	virtual void StoreByte( u16 address, u8 byte );

	u16 GetBankIndex( void ) const;
	void SetBankIndex( u16 index );
};

