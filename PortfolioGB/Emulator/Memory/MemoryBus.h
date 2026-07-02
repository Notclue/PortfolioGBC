#pragma once
#include "Emulator/HardwareRevision.h"
#include "MemoryMapper.h"
#include "MemoryProxy.h"

class MemoryBus {
public:
	MemoryBus( HardwareRevision rev );
	~MemoryBus();

	MemoryMapper* mainMemory;

	MemoryProxy* cpuProxy;
	MemoryProxy* ppuProxy;
	MemoryProxy* dmaProxy;
};

