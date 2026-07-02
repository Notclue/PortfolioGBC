#pragma once
#include "Types.h"
#include "Emulator/Memory/IMemory.h"

//TODO define constant for branch state.
#define NEXT_CYCLE state++; return Next
#define OPCODE_BRANCH state = 10; return Branch
#define END_OPCODE state = 0; EndOpcode(); return End

enum CPUFlagBits {
	Z = 7, //Zero flag
	N = 6, //Subtraction flag
	H = 5, //Half-carry flag
	C = 4, //Carry flag
};

enum OpcodeReturn {
	Next,
	Branch,
	End,
	Nothing //Used for opcodes that the Game Boy itself does not implement.
};

enum HardwareRevision {
	//TODO: fill this out
	DMG,
	CGB
};

class CPU {
private:
	//Allows registers to be accessed as a single 16 bit register or two 8 bit registers.
	struct CPURegisters {
		union {
			struct {
				u8 A, F;
				u8 B, C;
				u8 D, E;
				u8 H, L;
				u8 SPH, SPL;
				u8 W, Z; //WZ is an unoffical register that lives inside the ALU and is used as intermediate storage mid-opcode.
			};
			struct {
				u16 AF; //AF needs to be accessible as 16bit because you can push and pop it from the stack for some god forsaken reason.
				u16 BC;
				u16 DE;
				u16 HL;
				u16 SP;
				u16 WZ;
			};
		};

		u16 PC; //Pointer to the next instruction to execute.
		u8 IR; //ID of the next instruction to execute.

		bool IE; //Flag controlling execution of hardware interrupts.
	};
	CPURegisters registers;

	u8 state;
	IMemory* memoryBus;

	OpcodeReturn( CPU::* opcodeTableFunc )( u8 ) { &CPU::ExecuteOpcode };

	void EndOpcode();
	bool HandleInterrupts();
	OpcodeReturn ExecuteOpcode( u8 opcode );
	OpcodeReturn ExecuteOpcodeCB( u8 opcode );

	//Load from the location stored in the provided register.
	u8 DereferenceRegisterLoad( u16 reg ) const;
	//Store to the location pointed to by the provided register.
	void DereferenceRegisterStore( u16 reg, u8 data );

	//Opcodes
	OpcodeReturn CopyR8ToR8( u8& dest, u8 source );
	OpcodeReturn CopyR16PtrToR8( u8& dest, u16 source );
	OpcodeReturn CopyR8toR16Ptr( u16 dest, u8 source );
	OpcodeReturn CopyR8ToR16PtrPostInc( u16& dest, u8 source );
	OpcodeReturn CopyR8ToR16PtrPostDec( u16& dest, u8 source );
	OpcodeReturn CopyR16PtrToR8PostInc( u8& dest, u16 source );
	OpcodeReturn CopyR16PtrToR8PostDec( u8& dest, u16 source );

	OpcodeReturn CopyR8PtrToR8( u8& dest, u8 source );
	OpcodeReturn CopyR8ToR8Ptr( u8 dest, u8 source );

	OpcodeReturn CopyU8ToR8( u8& dest );
	OpcodeReturn CopyU8ToR16Ptr( u16 dest );
	OpcodeReturn CopyU16ToR16( u16& dest );
	OpcodeReturn CopyR16ToU16Ptr( u16 source );

	OpcodeReturn PushR16ToStack( u16 source );
	OpcodeReturn PopR16FromStack( u16& dest );


	//0xCB Opcodes
	OpcodeReturn CBPrefix();

	OpcodeReturn RotateRightCircularR8(u8& reg);
	OpcodeReturn RotateRightCircularR16Ptr(u16 reg);

	OpcodeReturn RotateLeftR8( u8& reg );
	OpcodeReturn RotateLeftR16Ptr( u16 reg );

	OpcodeReturn SetBitR8( u8 bit, u8& reg );
	OpcodeReturn SetBitR16Ptr( u8 bit, u16 reg );

	OpcodeReturn ResetBitR8( u8 bit, u8& reg );
	OpcodeReturn ResetBitR16Ptr( u8 bit, u16 reg );

	OpcodeReturn TestBitR8( u8 bit, u8 reg );
	OpcodeReturn TestBitR16Ptr( u8 bit, u16 reg );

public:
	CPU( IMemory* memoryBus );

	void OnMCycle();
};

