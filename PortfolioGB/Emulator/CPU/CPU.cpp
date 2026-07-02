#include "CPU.h"
#include "Types.h"

#include "OpcodesLoad.h"

u8 __forceinline CPU::DereferenceRegisterLoad( u16 reg ) const {
	return memoryBus->LoadByte( reg );
}

void __forceinline CPU::DereferenceRegisterStore( u16 reg, u8 data ) {
	memoryBus->StoreByte( reg, data );
}

void CPU::OnMCycle() {
	OpcodeReturn result = ( this->*opcodeTableFunc )( registers.IR );
}

void CPU::EndOpcode() {
	if(HandleInterrupts()) [[unlikely]]
		return;

	registers.IR = DereferenceRegisterLoad( registers.PC );
	registers.PC += 1;
}


bool CPU::HandleInterrupts() {
	if(!registers.IE)
		return false;

	return true;
}

//TODO: replace self-copy register ops with NOP?
//TODO: most self-operating instructions should have optimized versions
OpcodeReturn CPU::ExecuteOpcode( u8 opcode ) {
	//TODO: try using always inline annotation here
	switch(opcode) {
	case 0x00: return OpcodeReturn::End;									//NOP
	case 0x01: return CopyU16ToR16( registers.BC );							//LD BC, u16
	case 0x02: return CopyR8toR16Ptr( registers.BC, registers.A );			//LD (BC), A

	case 0x06: return CopyU8ToR8( registers.B );							//LD B, u8

	case 0x08: return CopyR16ToU16Ptr( registers.SP );						//LD (u16), SP

	case 0x0A: return CopyR16PtrToR8( registers.A, registers.BC );			//LD A, (BC)

	case 0x0E: return CopyU8ToR8( registers.C );							//LD C, u8

		//0x10 STOP
	case 0x11: return CopyU16ToR16( registers.DE );							//LD DE, u16
	case 0x12: return CopyR8toR16Ptr( registers.DE, registers.A );			//LD (DE), A

	case 0x16: return CopyU8ToR8( registers.D );							//LD D, u8

	case 0x1A: return CopyR16PtrToR8( registers.A, registers.DE );			//LD A, (DE)

	case 0x1E: return CopyU8ToR8( registers.E );							//LD E, u8

	case 0x21: return CopyU16ToR16( registers.HL );							//LD HL, u16
	case 0x22: return CopyR8ToR16PtrPostInc( registers.HL, registers.A );	//LD (HL+), A

	case 0x26: return CopyU8ToR8( registers.H );							//LD H, u8

	case 0x2A: return CopyR16PtrToR8PostInc( registers.A, registers.HL );	//LD A, (HL+)

	case 0x2E: return CopyU8ToR8( registers.L );							//LD L, u8

	case 0x31: return CopyU16ToR16( registers.SP );							//LD SP, u16
	case 0x32: return CopyR8ToR16PtrPostDec( registers.HL, registers.A );	//LD (HL-), A

	case 0x36: return CopyU8ToR16Ptr( registers.HL );						//LD (HL), u8

	case 0x3A: return CopyR16PtrToR8PostDec( registers.A, registers.HL );	//LD A, (HL-)

	case 0x3E: return CopyU8ToR8( registers.A );							//LD A, u8

	case 0x40: return CopyR8ToR8( registers.B, registers.B );				//LD B, B
	case 0x41: return CopyR8ToR8( registers.B, registers.C );				//LD B, C
	case 0x42: return CopyR8ToR8( registers.B, registers.D );				//LD B, D
	case 0x43: return CopyR8ToR8( registers.B, registers.E );				//LD B, E
	case 0x44: return CopyR8ToR8( registers.B, registers.H );				//LD B, H
	case 0x45: return CopyR8ToR8( registers.B, registers.L );				//LD B, L
	case 0x46: return CopyR16PtrToR8( registers.B, registers.HL );			//LD B, (HL)
	case 0x47: return CopyR8ToR8( registers.B, registers.A );				//LD B, A
	case 0x48: return CopyR8ToR8( registers.C, registers.B );				//LD C, B
	case 0x49: return CopyR8ToR8( registers.C, registers.C );				//LD C, C
	case 0x4A: return CopyR8ToR8( registers.C, registers.D );				//LD C, D
	case 0x4B: return CopyR8ToR8( registers.C, registers.E );				//LD C, E
	case 0x4C: return CopyR8ToR8( registers.C, registers.H );				//LD C, H
	case 0x4D: return CopyR8ToR8( registers.C, registers.L );				//LD C, L
	case 0x4E: return CopyR16PtrToR8( registers.C, registers.HL );			//LD C, (HL)
	case 0x4F: return CopyR8ToR8( registers.C, registers.A );				//LD C, A
	case 0x50: return CopyR8ToR8( registers.D, registers.B );				//LD D, B
	case 0x51: return CopyR8ToR8( registers.D, registers.C );				//LD D, C
	case 0x52: return CopyR8ToR8( registers.D, registers.D );				//LD D, D
	case 0x53: return CopyR8ToR8( registers.D, registers.E );				//LD D, E
	case 0x54: return CopyR8ToR8( registers.D, registers.H );				//LD D, H
	case 0x55: return CopyR8ToR8( registers.D, registers.L );				//LD D, L
	case 0x56: return CopyR16PtrToR8( registers.D, registers.HL );			//LD D, (HL)
	case 0x57: return CopyR8ToR8( registers.D, registers.A );				//LD D, A
	case 0x58: return CopyR8ToR8( registers.E, registers.B );				//LD E, B
	case 0x59: return CopyR8ToR8( registers.E, registers.C );				//LD E, C
	case 0x5A: return CopyR8ToR8( registers.E, registers.D );				//LD E, D
	case 0x5B: return CopyR8ToR8( registers.E, registers.E );				//LD E, E
	case 0x5C: return CopyR8ToR8( registers.E, registers.H );				//LD E, H
	case 0x5D: return CopyR8ToR8( registers.E, registers.L );				//LD E, L
	case 0x5E: return CopyR16PtrToR8( registers.E, registers.HL );			//LD E, (HL)
	case 0x5F: return CopyR8ToR8( registers.E, registers.A );				//LD E, A
	case 0x60: return CopyR8ToR8( registers.H, registers.B );				//LD H, B
	case 0x61: return CopyR8ToR8( registers.H, registers.C );				//LD H, C
	case 0x62: return CopyR8ToR8( registers.H, registers.D );				//LD H, D
	case 0x63: return CopyR8ToR8( registers.H, registers.E );				//LD H, E
	case 0x64: return CopyR8ToR8( registers.H, registers.H );				//LD H, H
	case 0x65: return CopyR8ToR8( registers.H, registers.L );				//LD H, L
	case 0x66: return CopyR16PtrToR8( registers.H, registers.HL );			//LD H, (HL)
	case 0x67: return CopyR8ToR8( registers.H, registers.A );				//LD H, A
	case 0x68: return CopyR8ToR8( registers.L, registers.B );				//LD L, B
	case 0x69: return CopyR8ToR8( registers.L, registers.C );				//LD L, C
	case 0x6A: return CopyR8ToR8( registers.L, registers.D );				//LD L, D
	case 0x6B: return CopyR8ToR8( registers.L, registers.E );				//LD L, E
	case 0x6C: return CopyR8ToR8( registers.L, registers.H );				//LD L, H
	case 0x6D: return CopyR8ToR8( registers.L, registers.L );				//LD L, L
	case 0x6E: return CopyR16PtrToR8( registers.L, registers.HL );			//LD L, (HL)
	case 0x6F: return CopyR8ToR8( registers.L, registers.A );				//LD L, A
	case 0x70: return CopyR8toR16Ptr( registers.HL, registers.B );			//LD (HL), B
	case 0x71: return CopyR8toR16Ptr( registers.HL, registers.C );			//LD (HL), C
	case 0x72: return CopyR8toR16Ptr( registers.HL, registers.D );			//LD (HL), D
	case 0x73: return CopyR8toR16Ptr( registers.HL, registers.E );			//LD (HL), E
	case 0x74: return CopyR8toR16Ptr( registers.HL, registers.H );			//LD (HL), H
	case 0x75: return CopyR8toR16Ptr( registers.HL, registers.L );			//LD (HL), L
		//0x76 HALT
	case 0x77: return CopyR8toR16Ptr( registers.HL, registers.A );			//LD (HL), A
	case 0x78: return CopyR8ToR8( registers.A, registers.B );				//LD A, B
	case 0x79: return CopyR8ToR8( registers.A, registers.C );				//LD A, C
	case 0x7A: return CopyR8ToR8( registers.A, registers.D );				//LD A, D
	case 0x7B: return CopyR8ToR8( registers.A, registers.E );				//LD A, E
	case 0x7C: return CopyR8ToR8( registers.A, registers.H );				//LD A, H
	case 0x7D: return CopyR8ToR8( registers.A, registers.L );				//LD A, L
	case 0x7E: return CopyR16PtrToR8( registers.A, registers.HL );			//LD A, (HL)
	case 0x7F: return CopyR8ToR8( registers.A, registers.A );				//LD A, A

	case 0xC1: return PopR16FromStack( registers.BC );						//POP BC

	case 0xC5: return PushR16ToStack( registers.BC );						//PUSH BC

	case 0xCB: return SetCBState();

	case 0xD1: return PopR16FromStack( registers.DE );						//POP DE
		//TODO: check all nothing opcode IDs
	case 0xD3: return OpcodeReturn::Nothing;								//Nothing

	case 0xD5: return PushR16ToStack( registers.DE );						//PUSH DE

	case 0xD8: return OpcodeReturn::Nothing;								//Nothing

	case 0xDD: return OpcodeReturn::Nothing;								//Nothing

	case 0xE0: return CopyI8PtrToR8( registers.A ); //TODO LD (FF00+u8), A
	case 0xE1: return PopR16FromStack( registers.HL );						//POP HL
	case 0xE2: return CopyR8ToR8Ptr( registers.C, registers.A );			//LDH (0xFF00 + C), A
	case 0xE3: return OpcodeReturn::Nothing;								//Nothing
	case 0xE4: return OpcodeReturn::Nothing;								//Nothing
	case 0xE5: return PushR16ToStack( registers.HL );						//PUSH HL

	case 0xE8: return OpcodeReturn::Nothing;								//Nothing

		//0xEA LD (u16), A

	case 0xEC: return OpcodeReturn::Nothing;								//Nothing
	case 0xED: return OpcodeReturn::Nothing;								//Nothing
	case 0xF0: return CopyR8ToI8Ptr( registers.A ); //TODO LD A, (FF00+u8)
	case 0xF1: return PopR16FromStack( registers.AF );						//POP AF
	case 0xF2: return CopyR8PtrToR8( registers.A, registers.C );			//LDH A, (0xFF00 + C)
		//0xF3 DI
	case 0xF4: return OpcodeReturn::Nothing;								//Nothing
	case 0xF5: return PushR16ToStack( registers.AF );						//PUSH AF

		//0xF8 LD HL, SP+i8
		//0xF9 LD SP, HL
		//0xFA LD A, (u16)

		//0xFB DI
	case 0xFC: return OpcodeReturn::Nothing;								//Nothing
	case 0xFD: return OpcodeReturn::Nothing;								//Nothing

	default:
		NODEFAULT;
	}
}

OpcodeReturn CPU::ExecuteOpcodeCB( u8 opcode ) {
	opcodeTableFunc = &CPU::ExecuteOpcode;

	switch(state) {
	case 0x00: return RotateLeftCircularR8( registers.B ); //RLC B
	case 0x01: return RotateLeftCircularR8( registers.C ); //RLC C
	case 0x02: return RotateLeftCircularR8( registers.D ); //RLC D
	case 0x03: return RotateLeftCircularR8( registers.E ); //RLC E
	case 0x04: return RotateLeftCircularR8( registers.H ); //RLC H
	case 0x05: return RotateLeftCircularR8( registers.L ); //RLC L
	case 0x06: return RotateLeftCircularR16Ptr( registers.HL ); //RLC (HL)
	case 0x07: return RotateLeftCircularR8( registers.A ); //RLC A

	case 0x08: return RotateRightCircularR8( registers.B ); //RRC B
	case 0x09: return RotateRightCircularR8( registers.C ); //RRC C
	case 0x0A: return RotateRightCircularR8( registers.D ); //RRC D
	case 0x0B: return RotateRightCircularR8( registers.E ); //RRC E
	case 0x0C: return RotateRightCircularR8( registers.H ); //RRC H
	case 0x0D: return RotateRightCircularR8( registers.L ); //RRC L
	case 0x0E: return RotateRightCircularR16Ptr( registers.HL ); //RRC (HL)
	case 0x0F: return RotateRightCircularR8( registers.A ); //RRC A

	case 0x10: return RotateLeftR8( registers.B ); //RL B
	case 0x11: return RotateLeftR8( registers.C ); //RL C
	case 0x12: return RotateLeftR8( registers.D ); //RL D
	case 0x13: return RotateLeftR8( registers.E ); //RL E
	case 0x14: return RotateLeftR8( registers.H ); //RL H
	case 0x15: return RotateLeftR8( registers.L ); //RL L
	case 0x16: return RotateLeftR16Ptr( registers.HL ); //RL (HL)
	case 0x17: return RotateLeftR8( registers.A ); //RL A

	case 0x18: return RotateRightR8( registers.B ); //RR B
	case 0x19: return RotateRightR8( registers.C ); //RR C
	case 0x1A: return RotateRightR8( registers.D ); //RR D
	case 0x1B: return RotateRightR8( registers.E ); //RR E
	case 0x1C: return RotateRightR8( registers.H ); //RR H
	case 0x1D: return RotateRightR8( registers.L ); //RR L
	case 0x1E: return RotateRightR16Ptr( registers.HL ); //RR (HL)
	case 0x1F: return RotateRightR8( registers.A ); //RR A

	case 0x40: return TestBitR8( 0, registers.B );							   //BIT 0, B
	case 0x41: return TestBitR8( 0, registers.C );							   //BIT 0, C
	case 0x42: return TestBitR8( 0, registers.D );							   //BIT 0, D
	case 0x43: return TestBitR8( 0, registers.E );							   //BIT 0, E
	case 0x44: return TestBitR8( 0, registers.H );							   //BIT 0, H
	case 0x45: return TestBitR8( 0, registers.L );							   //BIT 0, L
	case 0x46: return TestBitR16Ptr( 0, registers.HL );						   //BIT 0, (HL)
	case 0x47: return TestBitR8( 0, registers.A );							   //BIT 0, A
	case 0x48: return TestBitR8( 1, registers.B );							   //BIT 1, B
	case 0x49: return TestBitR8( 1, registers.C );							   //BIT 1, C
	case 0x4A: return TestBitR8( 1, registers.D );							   //BIT 1, D
	case 0x4B: return TestBitR8( 1, registers.E );							   //BIT 1, E
	case 0x4C: return TestBitR8( 1, registers.H );							   //BIT 1, H
	case 0x4D: return TestBitR8( 1, registers.L );							   //BIT 1, L
	case 0x4E: return TestBitR16Ptr( 1, registers.HL );						   //BIT 1, (HL)
	case 0x4F: return TestBitR8( 1, registers.A );							   //BIT 1, A
	case 0x50: return TestBitR8( 2, registers.B );							   //BIT 2, B
	case 0x51: return TestBitR8( 2, registers.C );							   //BIT 2, C
	case 0x52: return TestBitR8( 2, registers.D );							   //BIT 2, D
	case 0x53: return TestBitR8( 2, registers.E );							   //BIT 2, E
	case 0x54: return TestBitR8( 2, registers.H );							   //BIT 2, H
	case 0x55: return TestBitR8( 2, registers.L );							   //BIT 2, L
	case 0x56: return TestBitR16Ptr( 2, registers.HL );						   //BIT 2, (HL)
	case 0x57: return TestBitR8( 2, registers.A );							   //BIT 2, A
	case 0x58: return TestBitR8( 3, registers.B );							   //BIT 3, B
	case 0x59: return TestBitR8( 3, registers.C );							   //BIT 3, C
	case 0x5A: return TestBitR8( 3, registers.D );							   //BIT 3, D
	case 0x5B: return TestBitR8( 3, registers.E );							   //BIT 3, E
	case 0x5C: return TestBitR8( 3, registers.H );							   //BIT 3, H
	case 0x5D: return TestBitR8( 3, registers.L );							   //BIT 3, L
	case 0x5E: return TestBitR16Ptr( 3, registers.HL );						   //BIT 3, (HL)
	case 0x5F: return TestBitR8( 3, registers.A );							   //BIT 3, A
	case 0x60: return TestBitR8( 4, registers.B );							   //BIT 4, B
	case 0x61: return TestBitR8( 4, registers.C );							   //BIT 4, C
	case 0x62: return TestBitR8( 4, registers.D );							   //BIT 4, D
	case 0x63: return TestBitR8( 4, registers.E );							   //BIT 4, E
	case 0x64: return TestBitR8( 4, registers.H );							   //BIT 4, H
	case 0x65: return TestBitR8( 4, registers.L );							   //BIT 4, L
	case 0x66: return TestBitR16Ptr( 4, registers.HL );						   //BIT 4, (HL)
	case 0x67: return TestBitR8( 4, registers.A );							   //BIT 4, A
	case 0x68: return TestBitR8( 5, registers.B );							   //BIT 5, B
	case 0x69: return TestBitR8( 5, registers.C );							   //BIT 5, C
	case 0x6A: return TestBitR8( 5, registers.D );							   //BIT 5, D
	case 0x6B: return TestBitR8( 5, registers.E );							   //BIT 5, E
	case 0x6C: return TestBitR8( 5, registers.H );							   //BIT 5, H
	case 0x6D: return TestBitR8( 5, registers.L );							   //BIT 5, L
	case 0x6E: return TestBitR16Ptr( 5, registers.HL );						   //BIT 5, (HL)
	case 0x6F: return TestBitR8( 5, registers.A );							   //BIT 5, A
	case 0x70: return TestBitR8( 6, registers.B );							   //BIT 6, B
	case 0x71: return TestBitR8( 6, registers.C );							   //BIT 6, C
	case 0x72: return TestBitR8( 6, registers.D );							   //BIT 6, D
	case 0x73: return TestBitR8( 6, registers.E );							   //BIT 6, E
	case 0x74: return TestBitR8( 6, registers.H );							   //BIT 6, H
	case 0x75: return TestBitR8( 6, registers.L );							   //BIT 6, L
	case 0x76: return TestBitR16Ptr( 6, registers.HL );						   //BIT 6, (HL)
	case 0x77: return TestBitR8( 6, registers.A );							   //BIT 6, A
	case 0x78: return TestBitR8( 7, registers.B );							   //BIT 7, B
	case 0x79: return TestBitR8( 7, registers.C );							   //BIT 7, C
	case 0x7A: return TestBitR8( 7, registers.D );							   //BIT 7, D
	case 0x7B: return TestBitR8( 7, registers.E );							   //BIT 7, E
	case 0x7C: return TestBitR8( 7, registers.H );							   //BIT 7, H
	case 0x7D: return TestBitR8( 7, registers.L );							   //BIT 7, L
	case 0x7E: return TestBitR16Ptr( 7, registers.HL );						   //BIT 7, (HL)
	case 0x7F: return TestBitR8( 7, registers.A );							   //BIT 7, A

	case 0x80: return ResetBitR8( 0, registers.B );						   //RES 0, B
	case 0x81: return ResetBitR8( 0, registers.C );						   //RES 0, C
	case 0x82: return ResetBitR8( 0, registers.D );						   //RES 0, D
	case 0x83: return ResetBitR8( 0, registers.E );						   //RES 0, E
	case 0x84: return ResetBitR8( 0, registers.H );						   //RES 0, H
	case 0x85: return ResetBitR8( 0, registers.L );						   //RES 0, L
	case 0x86: return ResetBitR16Ptr( 0, registers.HL );				   //RES 0, (HL)
	case 0x87: return ResetBitR8( 0, registers.A );						   //RES 0, A
	case 0x88: return ResetBitR8( 1, registers.B );						   //RES 1, B
	case 0x89: return ResetBitR8( 1, registers.C );						   //RES 1, C
	case 0x8A: return ResetBitR8( 1, registers.D );						   //RES 1, D
	case 0x8B: return ResetBitR8( 1, registers.E );						   //RES 1, E
	case 0x8C: return ResetBitR8( 1, registers.H );						   //RES 1, H
	case 0x8D: return ResetBitR8( 1, registers.L );						   //RES 1, L
	case 0x8E: return ResetBitR16Ptr( 1, registers.HL );				   //RES 1, (HL)
	case 0x8F: return ResetBitR8( 1, registers.A );						   //RES 1, A
	case 0x90: return ResetBitR8( 2, registers.B );						   //RES 2, B
	case 0x91: return ResetBitR8( 2, registers.C );						   //RES 2, C
	case 0x92: return ResetBitR8( 2, registers.D );						   //RES 2, D
	case 0x93: return ResetBitR8( 2, registers.E );						   //RES 2, E
	case 0x94: return ResetBitR8( 2, registers.H );						   //RES 2, H
	case 0x95: return ResetBitR8( 2, registers.L );						   //RES 2, L
	case 0x96: return ResetBitR16Ptr( 2, registers.HL );				   //RES 2, (HL)
	case 0x97: return ResetBitR8( 2, registers.A );						   //RES 2, A
	case 0x98: return ResetBitR8( 3, registers.B );						   //RES 3, B
	case 0x99: return ResetBitR8( 3, registers.C );						   //RES 3, C
	case 0x9A: return ResetBitR8( 3, registers.D );						   //RES 3, D
	case 0x9B: return ResetBitR8( 3, registers.E );						   //RES 3, E
	case 0x9C: return ResetBitR8( 3, registers.H );						   //RES 3, H
	case 0x9D: return ResetBitR8( 3, registers.L );						   //RES 3, L
	case 0x9E: return ResetBitR16Ptr( 3, registers.HL );			 	   //RES 3, (HL)
	case 0x9F: return ResetBitR8( 3, registers.A );						   //RES 3, A
	case 0xA0: return ResetBitR8( 4, registers.B );						   //RES 4, B
	case 0xA1: return ResetBitR8( 4, registers.C );						   //RES 4, C
	case 0xA2: return ResetBitR8( 4, registers.D );						   //RES 4, D
	case 0xA3: return ResetBitR8( 4, registers.E );						   //RES 4, E
	case 0xA4: return ResetBitR8( 4, registers.H );						   //RES 4, H
	case 0xA5: return ResetBitR8( 4, registers.L );						   //RES 4, L
	case 0xA6: return ResetBitR16Ptr( 4, registers.HL );				   //RES 4, (HL)
	case 0xA7: return ResetBitR8( 4, registers.A );						   //RES 4, A
	case 0xA8: return ResetBitR8( 5, registers.B );						   //RES 5, B
	case 0xA9: return ResetBitR8( 5, registers.C );						   //RES 5, C
	case 0xAA: return ResetBitR8( 5, registers.D );						   //RES 5, D
	case 0xAB: return ResetBitR8( 5, registers.E );						   //RES 5, E
	case 0xAC: return ResetBitR8( 5, registers.H );						   //RES 5, H
	case 0xAD: return ResetBitR8( 5, registers.L );						   //RES 5, L
	case 0xAE: return ResetBitR16Ptr( 5, registers.HL );				   //RES 5, (HL)
	case 0xAF: return ResetBitR8( 5, registers.A );						   //RES 5, A
	case 0xB0: return ResetBitR8( 6, registers.B );						   //RES 6, B
	case 0xB1: return ResetBitR8( 6, registers.C );						   //RES 6, C
	case 0xB2: return ResetBitR8( 6, registers.D );						   //RES 6, D
	case 0xB3: return ResetBitR8( 6, registers.E );						   //RES 6, E
	case 0xB4: return ResetBitR8( 6, registers.H );						   //RES 6, H
	case 0xB5: return ResetBitR8( 6, registers.L );						   //RES 6, L
	case 0xB6: return ResetBitR16Ptr( 6, registers.HL );				   //RES 6, (HL)
	case 0xB7: return ResetBitR8( 6, registers.A );						   //RES 6, A
	case 0xB8: return ResetBitR8( 7, registers.B );						   //RES 7, B
	case 0xB9: return ResetBitR8( 7, registers.C );						   //RES 7, C
	case 0xBA: return ResetBitR8( 7, registers.D );						   //RES 7, D
	case 0xBB: return ResetBitR8( 7, registers.E );						   //RES 7, E
	case 0xBC: return ResetBitR8( 7, registers.H );						   //RES 7, H
	case 0xBD: return ResetBitR8( 7, registers.L );						   //RES 7, L
	case 0xBE: return ResetBitR16Ptr( 7, registers.HL );				   //RES 7, (HL)
	case 0xBF: return ResetBitR8( 7, registers.A );						   //RES 7, A

	case 0xC0: return SetBitR8( 0, registers.B );						   //BIT 0, B
	case 0xC1: return SetBitR8( 0, registers.C );						   //BIT 0, C
	case 0xC2: return SetBitR8( 0, registers.D );						   //BIT 0, D
	case 0xC3: return SetBitR8( 0, registers.E );						   //BIT 0, E
	case 0xC4: return SetBitR8( 0, registers.H );						   //BIT 0, H
	case 0xC5: return SetBitR8( 0, registers.L );						   //BIT 0, L
	case 0xC6: return SetBitR16Ptr( 0, registers.HL );					   //BIT 0, (HL)
	case 0xC7: return SetBitR8( 0, registers.A );						   //BIT 0, A
	case 0xC8: return SetBitR8( 1, registers.B );						   //BIT 1, B
	case 0xC9: return SetBitR8( 1, registers.C );						   //BIT 1, C
	case 0xCA: return SetBitR8( 1, registers.D );						   //BIT 1, D
	case 0xCB: return SetBitR8( 1, registers.E );						   //BIT 1, E
	case 0xCC: return SetBitR8( 1, registers.H );						   //BIT 1, H
	case 0xCD: return SetBitR8( 1, registers.L );						   //BIT 1, L
	case 0xCE: return SetBitR16Ptr( 1, registers.HL );					   //BIT 1, (HL)
	case 0xCF: return SetBitR8( 1, registers.A );						   //BIT 1, A
	case 0xD0: return SetBitR8( 2, registers.B );						   //BIT 2, B
	case 0xD1: return SetBitR8( 2, registers.C );						   //BIT 2, C
	case 0xD2: return SetBitR8( 2, registers.D );						   //BIT 2, D
	case 0xD3: return SetBitR8( 2, registers.E );						   //BIT 2, E
	case 0xD4: return SetBitR8( 2, registers.H );						   //BIT 2, H
	case 0xD5: return SetBitR8( 2, registers.L );						   //BIT 2, L
	case 0xD6: return SetBitR16Ptr( 2, registers.HL );					   //BIT 2, (HL)
	case 0xD7: return SetBitR8( 2, registers.A );						   //BIT 2, A
	case 0xD8: return SetBitR8( 3, registers.B );						   //BIT 3, B
	case 0xD9: return SetBitR8( 3, registers.C );						   //BIT 3, C
	case 0xDA: return SetBitR8( 3, registers.D );						   //BIT 3, D
	case 0xDB: return SetBitR8( 3, registers.E );						   //BIT 3, E
	case 0xDC: return SetBitR8( 3, registers.H );						   //BIT 3, H
	case 0xDD: return SetBitR8( 3, registers.L );						   //BIT 3, L
	case 0xDE: return SetBitR16Ptr( 3, registers.HL );					   //BIT 3, (HL)
	case 0xDF: return SetBitR8( 3, registers.A );						   //BIT 3, A
	case 0xE0: return SetBitR8( 4, registers.B );						   //BIT 4, B
	case 0xE1: return SetBitR8( 4, registers.C );						   //BIT 4, C
	case 0xE2: return SetBitR8( 4, registers.D );						   //BIT 4, D
	case 0xE3: return SetBitR8( 4, registers.E );						   //BIT 4, E
	case 0xE4: return SetBitR8( 4, registers.H );						   //BIT 4, H
	case 0xE5: return SetBitR8( 4, registers.L );						   //BIT 4, L
	case 0xE6: return SetBitR16Ptr( 4, registers.HL );					   //BIT 4, (HL)
	case 0xE7: return SetBitR8( 4, registers.A );						   //BIT 4, A
	case 0xE8: return SetBitR8( 5, registers.B );						   //BIT 5, B
	case 0xE9: return SetBitR8( 5, registers.C );						   //BIT 5, C
	case 0xEA: return SetBitR8( 5, registers.D );						   //BIT 5, D
	case 0xEB: return SetBitR8( 5, registers.E );						   //BIT 5, E
	case 0xEC: return SetBitR8( 5, registers.H );						   //BIT 5, H
	case 0xED: return SetBitR8( 5, registers.L );						   //BIT 5, L
	case 0xEE: return SetBitR16Ptr( 5, registers.HL );					   //BIT 5, (HL)
	case 0xEF: return SetBitR8( 5, registers.A );						   //BIT 5, A
	case 0xF0: return SetBitR8( 6, registers.B );						   //BIT 6, B
	case 0xF1: return SetBitR8( 6, registers.C );						   //BIT 6, C
	case 0xF2: return SetBitR8( 6, registers.D );						   //BIT 6, D
	case 0xF3: return SetBitR8( 6, registers.E );						   //BIT 6, E
	case 0xF4: return SetBitR8( 6, registers.H );						   //BIT 6, H
	case 0xF5: return SetBitR8( 6, registers.L );						   //BIT 6, L
	case 0xF6: return SetBitR16Ptr( 6, registers.HL );					   //BIT 6, (HL)
	case 0xF7: return SetBitR8( 6, registers.A );						   //BIT 6, A
	case 0xF8: return SetBitR8( 7, registers.B );						   //BIT 7, B
	case 0xF9: return SetBitR8( 7, registers.C );						   //BIT 7, C
	case 0xFA: return SetBitR8( 7, registers.D );						   //BIT 7, D
	case 0xFB: return SetBitR8( 7, registers.E );						   //BIT 7, E
	case 0xFC: return SetBitR8( 7, registers.H );						   //BIT 7, H
	case 0xFD: return SetBitR8( 7, registers.L );						   //BIT 7, L
	case 0xFE: return SetBitR16Ptr( 7, registers.HL );					   //BIT 7, (HL)
	case 0xFF: return SetBitR8( 7, registers.A );						   //BIT 7, A

	default:
		NODEFAULT;
	}
}