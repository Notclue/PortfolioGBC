#include "IMemory.h"
#include "MemoryBus.h"
#include "MemoryMapper.h"
#include "MemoryProxy.h"
#include "MemoryRange.h"
#include "MemoryRangeBanked.h"
#include <Emulator/HardwareRevision.h>

MemoryBus::MemoryBus( HardwareRevision rev ) {
	auto rom = MemoryRange( ROM_SIZE );
	auto vram = MemoryRangeBanked( VRAM_SIZE, 2 );
	auto sram = MemoryRangeBanked( SRAM_SIZE, 2 );
	auto wram1 = MemoryRange( WRAM1_SIZE );
	auto wram2 = MemoryRangeBanked( WRAM2_SIZE, 7 );

	auto echo = MemoryMapper( { &wram1, &wram2 }, ECHO_SIZE );

	auto oam = MemoryRange( OAM_SIZE );
	auto useless = MemoryRange( GARBAGE_SIZE );

	auto registers = MemoryRange( REGISTERS_SIZE );
	auto hram = MemoryRange( HRAM_SIZE );
	auto IE = MemoryRange( 1 );
	auto special = MemoryMapper( {
		&registers,
		&hram,
		&IE
	}
	);

	mainMemory = new MemoryMapper( {
		&rom,
		&vram,
		&sram,
		&wram1,
		&wram2,
		&echo,
		&oam,
		&useless,
		&registers,
		&special
	}
	);

	cpuProxy = new MemoryProxy( *mainMemory );
	ppuProxy = new MemoryProxy( *mainMemory );
	dmaProxy = new MemoryProxy( *mainMemory );
}

MemoryBus::~MemoryBus() {
	delete mainMemory;
	delete cpuProxy;
	delete ppuProxy;
	delete dmaProxy;
}