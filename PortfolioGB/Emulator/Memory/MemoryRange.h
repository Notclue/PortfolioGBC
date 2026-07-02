#pragma once
#include "IMemory.h"
#include "Types.h"

class MemoryRange : public IMemory {
private:
	u8* memory;
	u32 size;
	bool ownsMemory;

public:
	MemoryRange( u16 size );
	MemoryRange( u8* memory, u16 size );
	~MemoryRange( void );

	virtual u16 GetSize( void ) const;
	virtual u32 GetTrueSize( void ) const;

	virtual u8 LoadByte( u16 address ) const;
	virtual void StoreByte( u16 address, u8 byte );
};