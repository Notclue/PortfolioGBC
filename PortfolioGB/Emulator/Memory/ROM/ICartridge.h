#pragma once
#include "Emulator/Memory/IMemory.h"
#include "Types.h"

enum ROMMapperID {
	ICartridge,
	MBC1,
	MBC2,
	MBC3,
	MBC5,
	MBC6,
	MBC7,
	MMM01,
	HUC1,
	HUC3,
	TAMA5,
	CAMERA
};

enum ROMMapperFeature {
	RAM = 1 << 0,
	BATTERY = 1 << 1,
	TIMER = 1 << 2,
	RUMBLE = 1 << 3,
	SENSOR = 1 << 4,
};

struct ROMMapper {
	u8 hardwareID;
	ROMMapperID mapperID;
	u8 flags;
};

ROMMapper mapperTable[] = {
	{ 0x00, ROMMapperID::ICartridge, 0 },
	{ 0x01, ROMMapperID::MBC1, 0 },
	{ 0x02, ROMMapperID::MBC1, RAM },
	{ 0x03, ROMMapperID::MBC1, RAM | BATTERY },
	{ 0x05, ROMMapperID::MBC2, 0 },
	{ 0x06, ROMMapperID::MBC2, BATTERY },
	{ 0x05, ROMMapperID::MBC2, 0 },
};

class ICartridge {
public:
	virtual IMemory* getRomData() const = 0;
	virtual IMemory* getRamData() const = 0;

	//Return the amount of ROM space this ROM contains as defined by header byte 0x0148.
	virtual u32 getRomSize() const = 0;
	//Return the true amount of ROM space this ROM contains.
	virtual u32 getTrueRomSize() const = 0;

	//Return the amount of SRAM this ROM contains as defined by header byte 0x0149.
	virtual u32 getRamSize() const = 0;
	//Return the true amount of SRAM this ROM contains.
	virtual u32 getTrueRamSize() const = 0;

	static ICartridge* ROMFactory( const u8* romData, u32 romDataSize ) {

	}
};

