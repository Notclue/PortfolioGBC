#pragma once
#include "Types.h"

class ISerializable {
	virtual u8* Serialize() const = 0;
	virtual void Deserialize( const u8* data ) = 0;
};