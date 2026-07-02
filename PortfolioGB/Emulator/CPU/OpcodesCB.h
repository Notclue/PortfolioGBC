#pragma once
#include "CPU.h"
#include "Emulator/Bitmath.h"
#include "Types.h"
#include <intrin0.inl.h>

OpcodeReturn CPU::CBPrefix() {
	opcodeTableFunc = &CPU::ExecuteOpcodeCB;
	END_OPCODE;
}

OpcodeReturn CPU::SetBitR8( u8 bit, u8& reg ) {
	reg |= ( 1i8 << bit );
	END_OPCODE;
}

OpcodeReturn CPU::SetBitR16Ptr( u8 bit, u16 reg ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( reg );
		NEXT_CYCLE;
	case 1:
		DereferenceRegisterStore( reg, registers.Z | ( 1i8 << bit ) );
		NEXT_CYCLE;
	case 2:
		END_OPCODE;
	}
}

OpcodeReturn CPU::ResetBitR8( u8 bit, u8& reg ) {
	reg &= ~( 1i8 << bit );
	END_OPCODE;
}

OpcodeReturn CPU::ResetBitR16Ptr( u8 bit, u16 reg ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( reg );
		NEXT_CYCLE;
	case 1:
		DereferenceRegisterStore( reg, registers.Z & ~( 1i8 << bit ) );
		NEXT_CYCLE;
	case 2:
		END_OPCODE;
	}
}

OpcodeReturn CPU::TestBitR8( u8 bit, u8 reg ) {
	SetBitTo( registers.F, CPUFlagBits::Z, !BitIsSet( bit, reg ) );
	//TODO: can optimize multiple constant sets
	SetBitTo( registers.F, CPUFlagBits::N, false );
	SetBitTo( registers.F, CPUFlagBits::H, true );
	END_OPCODE;
}

OpcodeReturn CPU::TestBitR16Ptr( u8 bit, u16 reg ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( reg );
		NEXT_CYCLE;
	case 1:
		SetBitTo( registers.F, CPUFlagBits::Z, !BitIsSet( bit, registers.Z ) );
		//TODO: can optimize multiple constant sets
		SetBitTo( registers.F, CPUFlagBits::N, false );
		SetBitTo( registers.F, CPUFlagBits::H, true );
		END_OPCODE;
	}
}

OpcodeReturn CPU::RotateRightCircularR8(u8& reg) {
	bool bitZeroSet = BitIsSet( reg, 0 );

	reg = _rotr8( reg, 1 );

	SetBitTo( registers.F, CPUFlagBits::Z, reg == 0 );
	//TODO: can optimize multiple constant sets
	SetBitTo( registers.F, CPUFlagBits::N, false );
	SetBitTo( registers.F, CPUFlagBits::H, false );
	SetBitTo( registers.F, CPUFlagBits::C, bitZeroSet );
	END_OPCODE;
}

OpcodeReturn CPU::RotateRightCircularR16Ptr(u16 reg) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( reg );
		NEXT_CYCLE;
	case 1: {
		bool bitZeroSet = BitIsSet( registers.Z, 0 );

		registers.Z = _rotr8( registers.Z, 1 );

		SetBitTo( registers.F, CPUFlagBits::Z, reg == 0 );
		//TODO: can optimize multiple constant sets
		SetBitTo( registers.F, CPUFlagBits::N, false );
		SetBitTo( registers.F, CPUFlagBits::H, false );
		SetBitTo( registers.F, CPUFlagBits::C, bitZeroSet );

		DereferenceRegisterStore( reg, registers.Z );
		NEXT_CYCLE;
	}
	case 2:
		END_OPCODE;
	}
}

OpcodeReturn CPU::RotateLeftR8( u8& reg ) {
	bool lastBitSet = BitIsSet( reg, 7 );

	reg <<= 1;
	SetBitTo( reg, 0, BitIsSet( registers.F, CPUFlagBits::C ) );

	SetBitTo( registers.F, CPUFlagBits::Z, reg == 0 );
	//TODO: can optimize multiple constant sets
	SetBitTo( registers.F, CPUFlagBits::N, false );
	SetBitTo( registers.F, CPUFlagBits::H, false );
	SetBitTo( registers.F, CPUFlagBits::C, lastBitSet );
	END_OPCODE;
}

OpcodeReturn CPU::RotateLeftR16Ptr( u16 reg ) {
	switch(state) {
	case 0:
		registers.Z = DereferenceRegisterLoad( reg );
		NEXT_CYCLE;
	case 1: {
		bool lastBitSet = BitIsSet( registers.Z, 7 );

		registers.Z <<= 1;
		SetBitTo( registers.Z, 0, BitIsSet( registers.F, CPUFlagBits::C ) );

		SetBitTo( registers.F, CPUFlagBits::Z, reg == 0 );
		//TODO: can optimize multiple constant sets
		SetBitTo( registers.F, CPUFlagBits::N, false );
		SetBitTo( registers.F, CPUFlagBits::H, false );
		SetBitTo( registers.F, CPUFlagBits::C, lastBitSet );

		DereferenceRegisterStore( reg, registers.Z );
		NEXT_CYCLE;
	}
	case 2:
		END_OPCODE;
	}
}
