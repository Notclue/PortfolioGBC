#pragma once
#include "CPU.h"
#include "Types.h"
#include <Emulator/Bitmath.h>

OpcodeReturn CPU::CopyR8ToR8( u8& dest, u8 source ) {
	dest = source;
	END_OPCODE;	//TODO: single cycle function does not require setting state because it is already 0
}

OpcodeReturn CPU::CopyR16PtrToR8( u8& dest, u16 source ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( source );
		NEXT_CYCLE;
	case 1:
		dest = registers.Z;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR8toR16Ptr( u16 dest, u8 source ) {
	switch(state) {
	case 0:
		DereferenceRegisterStore( dest, source );
		NEXT_CYCLE;
	case 1:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR8ToR16PtrPostInc( u16& dest, u8 source ) {
	switch(state) {
	case 0:
		DereferenceRegisterStore( dest, source );
		dest++;
		NEXT_CYCLE;
	case 1:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR8ToR16PtrPostDec( u16& dest, u8 source ) {
	switch(state) {
	case 0:
		DereferenceRegisterStore( dest, source );
		dest++;
		NEXT_CYCLE;
	case 1:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR16PtrToR8PostInc( u8& dest, u16 source ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( source );
		dest++;
		NEXT_CYCLE;
	case 1:
		registers.A = registers.Z;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR16PtrToR8PostDec( u8& dest, u16 source ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( source );
		source++;
		NEXT_CYCLE;
	case 1:
		registers.A = registers.Z;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

inline OpcodeReturn CPU::CopyR8PtrToR8( u8& dest, u8 source ) {
	switch(state) {
	case 0:
		registers.Z = memoryBus.LoadByte( 0xFF00 | static_cast<u16>( source ) );
		NEXT_CYCLE;
	case 1:
		dest = registers.Z;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

inline OpcodeReturn CPU::CopyR8ToR8Ptr( u8 dest, u8 source ) {
	switch(state) {
	case 0:
		memoryBus.StoreByte( 0xFF00 | static_cast<u16>( source ), source ); //TODO this is wrong
		NEXT_CYCLE;
	case 1:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyU8ToR8( u8& dest ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 1:
		dest = registers.Z;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyU8ToR16Ptr( u16 dest ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 1:
		DereferenceRegisterStore( dest, registers.Z );
		NEXT_CYCLE;
	case 2:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyU16ToR16( u16& dest ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 1:
		registers.W = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 2:
		dest = registers.WZ;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::CopyR16ToU16Ptr(u16 source) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 1:
		registers.W = DereferenceRegisterLoad( registers.PC );
		registers.PC++;
		NEXT_CYCLE;
	case 2:
		DereferenceRegisterStore( registers.WZ, GetWordLowerByte( source ) );
		registers.WZ++;
		NEXT_CYCLE;
	case 3:
		DereferenceRegisterStore( registers.WZ, GetWordUpperByte( source ) );
		NEXT_CYCLE;
	case 4:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::PushR16ToStack( u16 source ) {
	switch(state) {
	case 0:
		registers.SP--;
		NEXT_CYCLE;
	case 1:
		DereferenceRegisterStore( registers.SP, GetWordUpperByte( source ) );
		registers.SP--;
		NEXT_CYCLE;
	case 2:
		DereferenceRegisterStore( registers.SP, GetWordLowerByte( source ) );
		NEXT_CYCLE;
	case 3:
		END_OPCODE;
	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::PopR16FromStack( u16& dest ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( registers.SP );
		registers.SP++;
		NEXT_CYCLE;
	case 1:
		registers.W = DereferenceRegisterLoad( registers.SP );
		registers.SP++;
		NEXT_CYCLE;
	case 2:
		dest = registers.WZ;
		END_OPCODE;
	default:
		NODEFAULT;
	}
}
