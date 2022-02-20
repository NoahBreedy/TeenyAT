/*
 * Name	   : teenyat.h
 * Author	 : William "Amos" Confer
 *
 * License	: Copyright (C) 2022 All rights reserved
 */

#ifndef __TEENYAT_H__
#define __TEENYAT_H__

#ifndef __cplusplus

#include <stdint.h>
#include <stdbool.h>

#else  // __cplusplus
extern "C" {

#include <cstdint>

#endif // __cplusplus

typedef struct teenyat teenyat;

typedef uint16_t tny_uword;
typedef int16_t tny_sword;
typedef union tny_word tny_word;

/** While the TeenyAT has a 16 bit address space, RAM is only 32K words */
#define TNY_RAM_SIZE (1 << 15)

struct teenyat {
	/** Has this TeenyAT ever been initialized */
	bool initialized;
	/** Memory used for a program's code/data */
	tny_word ram[TNY_RAM_SIZE];
	/** copy of original bin file for resets */
	tny_word bin_image[TNY_RAM_SIZE];
	/**
	 * Registers...
	 *
	 * reg[0]: Program Counter (PC)
	 *
	 * reg[1]: Stack Pointer (SP)... address of the next top
	 *
	 * reg[2]: Zero Register (rZ)... always contains zero and is read only
	 *
	 * reg[3]-[7]: General Purpose Registers (rA -- rE)
	 */
	tny_word reg[8];
	/**
	 * Flag bits are set by CMP and all ALU instructions.
	 */
	struct {
		/**
		 * Carry is set/cleared by arithmetic, shift and rotate instructions.
		 * For shift/rotate instructions, the final bit shifted out of storage
		 * determines the flag.  If the shift length is zero, the flag is
		 * unchanged. */
		bool carry;
		/** Set/cleared if the result of a CMP or ALU instruction is 0 or not */
		bool equals;
		/** This flag is effectively the sign bit of the CMP or ALU result */
		bool less;
		/** Set if CMP or ALU result is positive and non-zero */
		bool greater;
	} flags;
	/** The first encoded word of the current instruction */
	tny_word instruction;
	/**
	 * The immediate to be used by the instruction.  May come from the immed4
	 * field if the instruction is teeny, otherwise it comes from a second
	 * instruction word.
	 */
	tny_word immed;
	/** Stores information about current bus requests */
	struct {
		/** The target device address of the most recent request */
		tny_uword address;
		/** The data written to or read from the most recent request */
		tny_word data;
		/** 'R': reading/LOD, 'W': writing/STR, 'X': no request */
		char state;
	} bus;
	/**
	 * The number of remaining cycles to delay to simulate the cost of the
	 * previous instruction.
	 */
	unsigned int delay_cycles;
};

union tny_word {
	struct {
		int16_t immed4  : 4;
		uint16_t reg2   : 3;
		uint16_t reg1   : 3;
		uint16_t teeny  : 1;
		uint16_t opcode : 5;
	} instruction;

	struct {
		uint16_t greater  : 1;
		uint16_t less     : 1;
		uint16_t equals   : 1;
		uint16_t carry    : 1;
		uint16_t reserved : 12;
	} inst_flags;

	struct {
		uint16_t byte0 : 8;
		uint16_t byte1 : 8;
	} bytes;

	tny_uword u;
	tny_sword s;

	struct {
		uint16_t bit0  : 1;
		uint16_t bit1  : 1;
		uint16_t bit2  : 1;
		uint16_t bit3  : 1;
		uint16_t bit4  : 1;
		uint16_t bit5  : 1;
		uint16_t bit6  : 1;
		uint16_t bit7  : 1;
		uint16_t bit8  : 1;
		uint16_t bit9  : 1;
		uint16_t bit10 : 1;
		uint16_t bit11 : 1;
		uint16_t bit12 : 1;
		uint16_t bit13 : 1;
		uint16_t bit14 : 1;
		uint16_t bit15 : 1;
	} bits;
};

#define TNY_OPCODE_SET 0
#define TNY_OPCODE_LOD 1
#define TNY_OPCODE_STR 2
#define TNY_OPCODE_PSH 3
#define TNY_OPCODE_POP 4
#define TNY_OPCODE_BTS 5
#define TNY_OPCODE_BTC 6
#define TNY_OPCODE_BTF 7
#define TNY_OPCODE_CAL 8
#define TNY_OPCODE_ADD 9
#define TNY_OPCODE_SUB 10
#define TNY_OPCODE_MPY 11
#define TNY_OPCODE_DIV 12
#define TNY_OPCODE_MOD 13
#define TNY_OPCODE_AND 14
#define TNY_OPCODE_OR  15
#define TNY_OPCODE_XOR 16
#define TNY_OPCODE_SHF 17
#define TNY_OPCODE_ROT 18
#define TNY_OPCODE_NEG 19
#define TNY_OPCODE_CMP 20
#define TNY_OPCODE_JMP 21
#define TNY_OPCODE_DJZ 22

#define TNY_REG_PC   0
#define TNY_REG_SP   1
#define TNY_REG_ZERO 2
#define TNY_REG_A    3
#define TNY_REG_B    4
#define TNY_REG_C    5
#define TNY_REG_D    6
#define TNY_REG_E    7

/**
 * @brief
 *   Initialize a TeenyAT instance and buffer the file for future resets.
 *
 * @param t
 *   The TeenyAT instance to initialize
 *
 * @param bin_file
 *   The pre-assembled .bin file to load and execute
 *
 * @return
 *   True on success, flase otherwise.
 *
 * @note
 *   Upon failed initialization, the t->initialized member can be assumed false,
 *   but the state of all other members is undefined.
 */
bool tny_init_from_file(teenyat *t, FILE *bin_file);

/**
 * @brief
 *   Reinitialize the TeenyAT
 *
 * Restore the TeenyAT to its initialized state as if it had just been done so
 * from the original .bin file
 *
 * @param t
 *   The TeenyAT instance to reset
 *
 * @return
 *   True on success, flase otherwise.
 *   Attempting to reset an unitialized TeenyAT will always return false.
 */
bool tny_reset(teenyat *t);

/**
 * @brief
 *   Advance the TeenyAT instance by one clock cycle
 *
 * This function provides the TeenyAT with something similar to a CPU clock
 * pulse.  The TeenyAT will handle all the internals of the traditional Fetch,
 * Decode, and Execute processes, but it is the caller's responsibility to
 * check whether an any bus requests were made after EVERY cycle.
 *
 * The caller MUST check t's bus state for whether the access is a read or
 * write.  Read/writes from/to external devices must be handled immediately...
 * before calling tny_clock() again.  Check the character in t->bus.state to
 * determine the type of bus access request.  'R' indicates t was trying to read
 * from the device at the address t->bus.address.  'W' indicates t was trying to
 * write the data in t->bus.data to the device at the address t->bus.address.
 * the state member may also contain 'X', indicating no request at all.
 *
 * On read requests, the caller MUST satisfy the request via tny_lod_result().
 *
 * On write requests, the caller MUST satisfy the request via tny_str_result().
 *
 * @param t
 *   The TeenyAT instance
 *
 * @return
 *   True if the TeenyAT instance is making a bus request, false otherwise.
 */
bool tny_clock(teenyat *t);

/**
 * @brief
 *   Satisfy a LOD (read from a device) request from the TeenyAT
 *
 * @param t
 *   The TeenyAT instance
 *
 * @param data
 *   The data being sent back to the TeenyAT
 *
 * @param delay
 *   Use this to tell the TeenyAT how many additional cycles of cost you want
 *   associated with this read request.  For example, if you want the device
 *   being read from to have a penalty of 9 clock cycles on top of the particular
 *   LOD instruction used, pass 9 as the delay.  The TeenyAT will ensure this
 *   many cycles pass before the next instruction is executed.
 *
 * @note
 *   It is important the caller has already determined both the device being
 *   read from via its address before calling this function.  This
 *   can be acquired from t->bus.address.
 */
void tny_lod_result(teenyat *t, tny_word data, uint16_t delay);

/**
 * @brief
 *   Satisfy a STR (write to a device) request from the TeenyAT
 *
 * @param t
 *   The TeenyAT instance
 *
 * @param delay
 *   Use this to tell the TeenyAT how many additional cycles of cost you want
 *   associated with this write request.  For example, if you want the device
 *   being written to to have a penalty of 7 clock cycles on top of the particular
 *   STR instruction used, pass 7 as the delay.  The TeenyAT will ensure this
 *   many cycles pass before the next instruction is executed.
 *
 * @note
 *   It is important the caller has already determined both the device being
 *   written to as well as the data to write before calling this function.  These
 *   can be acquired from t->bus.address and t->bus.data, respectively.
 */
void tny_str_result(teenyat *t, uint16_t delay);

#ifdef __cplusplus
}
#endif

#endif /* __TEENYAT_H__ */
