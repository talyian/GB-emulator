#include <cstdint>
#include <cstdio>
#include <memory>
#include "unistd.h"

#include "registers.hh"
void Registers::dump() {
  printf(" A: %02hhx  \t", A);
  printf(" B: %02hhx  \t", B);
  printf(" C: %02hhx  \t", C);
  printf(" D: %02hhx  \t", D);
  printf(" E: %02hhx  \t", E);
  printf(" F: %02hhx  \n", F);
  printf("HL: %04hx\t", (u16) HL);
  printf("SP: %04hx\t", SP);
  printf("PC: %04hx\n", PC);
}

#include "main.hh"

u8 getRegister(REG8 r, Registers &rr) {
  switch(r) {
  #define F(Z) case REG8::Z: return rr.Z;
  FORALL_REG8(F)
  #undef F
  }
};
void setRegister(REG8 r, Registers &rr, u8 value) {
  switch(r) {
  #define F(Z) case REG8::Z: rr.Z = value; break;
  FORALL_REG8(F)
  #undef F
  }
}
u16 getRegister(REG16 r, Registers &rr) {
  switch(r) {
  #define F(Z) case REG16::Z: return rr.Z;
  FORALL_REG16(F)
  F(PC)
  F(SP)
  #undef F
  }
};

void setRegister(REG16 r, Registers &rr, u16 value) {
  switch(r) {
  #define F(Z) case REG16::Z: rr.Z = value; break;
  FORALL_REG16(F)
  F(PC)
  F(SP)
  #undef F
  }
}

u8 Val8::get(Registers &reg, Memory &mem)  {
  switch(type) {
  case Reg: return getRegister(value.r, reg);
  case Val: return value.value;
  case PtrN: return mem[value.ptr_n];
  case PtrR: return 0; // TODO
  case IoN: return mem[0xFF00 + value.io_n];
  case IoR: return mem[0xFF00 + getRegister(value.io_r, reg)];
  }
}

void Val8::set(Registers &reg, Memory &mem, Val8 source)  {
  u8 new_value = source.get(reg, mem);
  switch(type) {
  case Reg: setRegister(value.r, reg, new_value); break;
  case Val: abort(); // TODO
  case PtrN: mem[value.ptr_n] = new_value; break;
  case PtrR: mem[getRegister(value.ptr_r, reg)] = new_value; break;
  case IoN: mem[0xFF00 + value.io_n] = new_value; break;
  case IoR: mem[0xFF00 + getRegister(value.io_r, reg)] = new_value; break;
  }
}

u16 Val16::get(Registers &reg, __attribute__((unused)) Memory &mem) {
  switch(type) {
  case Reg: return getRegister(value.r, reg);
  case Val: return value.value;
  }
}

void Val16::set(Registers &reg, Memory &mem, Val16 source) {
  u16 new_value = source.get(reg, mem);
  switch(type) {
  case Reg: setRegister(value.r, reg, new_value); break;
  case Val: abort(); // TODO
  }
}

u8 Executor::get(Val8 a) { return a.get(reg, mem); }
u16 Executor::get(Val16 a) { return a.get(reg, mem); }
void Executor::set(Val8 a, Val8 v) { a.set(reg, mem, v); }
void Executor::set(Val16 a, Val16 v) { a.set(reg, mem, v); }

void Executor::NOP() { }
void Executor::HALT() { } // TODO

void Executor::LD(Val8 dst, Val8 src) { set(dst, get(src)); }
void Executor::LD(Val16 dst, Val16 src) { set(dst, get(src)); }

// stack operations
void Executor::PUSH(Val16 val) {
  u16 v = get(val);
  mem[reg.SP--] = v;
  mem[reg.SP--] = v >> 8;
}
u16 Executor::PEEK() {
  u16 v = 0;
  v = (v << 8) | mem[reg.SP + 1];
  v = (v << 8) | mem[reg.SP + 2];
  return v;
}
void Executor::POP(Val16 addr) {
  u16 v = 0;
  v = (v << 8) | mem[++reg.SP];
  v = (v << 8) | mem[++reg.SP];
  set(addr, v);
}

// jumps
void Executor::JP(Cond cond, Val16 dst) {
  // printf("JP %04x\n", get(dst));
  if (cond_eval(cond)) {
    reg.PC = get(dst); } }
void Executor::JR(Cond cond, Val8 offset) {
  // printf("JR ");
  // show(cond);
  // printf(" %04x\n", reg.PC + (int8_t)get(offset));
  if (cond_eval(cond)) { reg.PC += (int8_t)get(offset); }}
void Executor::RET(Cond cond) {
  printf("RET %04x\n", PEEK());
  if (cond_eval(cond)) { POP(REG16::PC); }}
void Executor::CALL(Cond cond, Val16 target) {
  printf("CALL %04x\n", get(target));
  if (cond_eval(cond)) {
    PUSH(REG16::PC);
    reg.PC = get(target);
  }}
// tests
void Executor::CP(Val8 rhs) {
  u8 a = reg.A;
  u8 b = get(rhs);
  reg.setFZ(a == b ? 1 : 0);
  reg.setFC(a < b ? 1 : 0);
  reg.setFH(0); // TODO:
  reg.setFO(1); // TODO: really??
}
void Executor::INC(Val8 dst) {
  // printf("INC ");
  // dst.show();
  // printf(" -> %x\n", get(dst) + 1);
  set(dst, get(dst) + 1); }
void Executor::INC(Val16 dst) { set(dst, get(dst) + 1); }
void Executor::DEC(Val8 dst) {
  // printf("DEC ");
  // dst.show();
  // printf(" -> %x\n", get(dst) - 1);
  set(dst, get(dst) - 1); }
void Executor::DEC(Val16 dst) {
  // printf("DEC ");
  // dst.show();
  // printf(" -> %x\n", get(dst) - 1);
  set(dst, get(dst) - 1); }

// bitwise operations
// TODO: the 1-byte opcodes RLCA RLA RRA RRCA don't set zero-flag?
// 9-bit Rotate right
void Executor::RR(Val8 v) {
  u8 val = get(v);
  u8 new_val = (val >> 1) | (reg.FC() << 7);
  reg.F = 0;
  reg.setFZ(new_val == 0);
  reg.setFC(val & 1);
  set(v, val);
}
// 8-bit rotate right
void Executor::RRC(Val8 v) {
  u8 val = get(v);
  val = (val << 7) | (val >> 1);
  reg.F = 0;
  reg.setFZ(val == 0);
  reg.setFC(val & 0x80); // old bit 7 is now bit 6
  set(v, val);
}
// 9-bit shift
void Executor::RL(Val8 v) {
  u8 val = get(v);
  u8 carry = (val >> 7) & 1;
  val = (val << 1) | (reg.FC());
  reg.setFZ(val == 0);
  reg.setFC(carry);
  reg.setFO(0);
  reg.setFH(0); // really?
  // reg.F = (val == 0 ? 0x80 : 0) | (carry ? 0x10 : 0);
  set(v, val);
}
// 8-bit shift
void Executor::RLC(Val8 v) {
  u8 val = get(v);
  u8 carry = (val >> 7) & 1;
  val = (val << 1) | carry;
  set(v, val);
  // reg.F = (val == 0 ? 0x80 : 0) | (carry ? 0x10 : 0);
  reg.setFZ(val == 0);
  reg.setFC(carry);
  reg.setFO(0);
  reg.setFH(0); // really?
}
// shift left
void Executor::SLA(Val8 val) {
  u8 v = get(val);
  reg.F = 0;
  reg.setFZ((v << 1) == 0);
  reg.setFC(v >> 7);
  set(val, v << 1);
}
// shift right - signed
void Executor::SRA(Val8 val) {
  int8_t v = get(val);
  reg.F = 0;
  reg.setFZ((v >> 1) == 0);
  reg.setFC(v & 1);
  set(val, v >> 1);
}
// shift right
void Executor::SRL(Val8 val) {
  u8 v = get(val);
  reg.F = 0;
  reg.setFZ((v >> 1) == 0);
  reg.setFC(v & 1);
  set(val, v >> 1);
}
// void Executor::SWAP(Val8 val);

// // single-bit
void Executor::RES(int bit, Val8 val) { set(val, get(val) & ~(1 << bit)); }
void Executor::SET(int bit, Val8 val) { set(val, get(val) | (1 << bit)); }
void Executor::BIT(int bit, Val8 rhs) {
  u8 val = get(rhs);
  u8 val2 = (val >> bit) & 1;
  reg.setFZ(val2 == 0);
  reg.setFO(0);
  reg.setFH(1); // TODO: really???
}

// // Arithmetic
void Executor::XOR(Val8 val) { reg.A ^= get(val); }
void Executor::ADD(Val8 dst, Val8 val) { set(dst, get(dst) + get(val)); }
void Executor::ADC(Val8 dst, Val8 val) { set(dst, get(dst) + get(val) + reg.FC()); }
void Executor::ADD(Val16 dst, Val16 val) { set(dst, get(dst) + get(val)); }
void Executor::SUB(Val8 val) { reg.A -= get(val); }
void Executor::SBC(Val8 val) { reg.A -= get(val) + reg.FC(); }

template<class Executor>
struct OpParser {
  Registers &registers;
  Memory &mem;
  Executor &ii;

  u8 * buf;
  size_t buflen;

  #define SS(X) REG8 X = REG8::X;
  FORALL_REG8(SS)
  #undef SS
  #define SS(X) REG16 X = REG16::X;
  FORALL_REG16(SS)
  SS(SP) SS(PC)
  #undef SS

  OpParser(Registers &registers, Memory &mem, Executor &ii, u8 * buf, size_t buflen)
  : registers(registers), mem(mem), ii(ii), buf(buf), buflen(buflen)  { }

  OpParser(Registers &registers, Memory &mem, Executor &ii, FILE * file)
    : registers(registers), mem(mem), ii(ii) {
    fseek(file, 0, SEEK_END);
    buflen = ftell(file);
    buf = (uint8_t *)malloc(buflen);
    fseek(file, 0, 0);
    fread(buf, 1, buflen, file);
  }

  Val8 Load(u16 x) { Val8 v(0); v.type=Val8::PtrN; v.value.ptr_n=x; return v; }
  Val8 Load(REG16 x) { Val8 v(0); v.type=Val8::PtrR; v.value.ptr_r=x; return v; }
  Val8 IO(u8 x) { Val8 v(0); v.type=Val8::IoN; v.value.io_n=x; return v; }
  Val8 IO(REG8 x) { Val8 v(0); v.type=Val8::IoR; v.value.io_r=x; return v; }
  // The parser decodes a single instruction and feeds it to a given interpreter
  void Step() {
    ii.timer = 0;
    registers._PC = registers.PC;
    u16 op = read8();
    if (op == 0xCB) { op = 0x100 | read8(); }
#define OP(CMD, op_len, op_time, flags) {ii.CMD;ii.timer+=op_time;break;}
#define TODO_OP(CMD, op_len, op_time, flags) { break; }
    switch (op) {
    case 0x00: OP(NOP()               ,  1,  4, "- - - -");
    case 0x01: OP(LD(BC,read16())     ,  3, 12, "- - - -");
    case 0x02: OP(LD(Load(BC),A)      ,  1,  8, "- - - -");
    case 0x03: OP(INC(BC)             ,  1,  8, "- - - -");
    case 0x04: OP(INC(B)              ,  1,  4, "Z 0 H -");
    case 0x05: OP(DEC(B)              ,  1,  4, "Z 1 H -");
    case 0x06: OP(LD(B,read8())       ,  2,  8, "- - - -");
    case 0x07: OP(RLC(A)              ,  1,  4, "0 0 0 C"); // fast RLC
      // case 0x08: OP(LD(Load(read16()),SP),  3, 20, "- - - -");
    case 0x09: OP(ADD(HL,BC)          ,  1,  8, "- 0 H C");
    case 0x0a: OP(LD(A,Load(BC))      ,  1,  8, "- - - -");
    case 0x0b: OP(DEC(BC)             ,  1,  8, "- - - -");
    case 0x0c: OP(INC(C)              ,  1,  4, "Z 0 H -");
    case 0x0d: OP(DEC(C)              ,  1,  4, "Z 1 H -");
    case 0x0e: OP(LD(C,read8())       ,  2,  8, "- - - -");
    // case 0x0f: OP(RRCA()              ,  1,  4, "0 0 0 C");
    case 0x0f: ii.RRC(A); break;
    // case 0x10: OP(STOP()              ,  2,  4, "- - - -");
    case 0x11: OP(LD(DE,read16())     ,  3, 12, "- - - -");
    case 0x12: OP(LD(Load(DE),A)      ,  1,  8, "- - - -");
    case 0x13: OP(INC(DE)             ,  1,  8, "- - - -");
    case 0x14: OP(INC(D)              ,  1,  4, "Z 0 H -");
    case 0x15: OP(DEC(D)              ,  1,  4, "Z 1 H -");
    case 0x16: OP(LD(D,read8())       ,  2,  8, "- - - -");
    // case 0x17: OP(RLA()               ,  1,  4, "0 0 0 C");
    case 0x17: ii.RL(A); break;
    case 0x18: OP(JR(Cond::ALWAYS,(int8_t)read8()),  2, 12, "- - - -");
    case 0x19: OP(ADD(HL,DE)          ,  1,  8, "- 0 H C");
    case 0x1a: OP(LD(A,Load(DE))      ,  1,  8, "- - - -");
    case 0x1b: OP(DEC(DE)             ,  1,  8, "- - - -");
    case 0x1c: OP(INC(E)              ,  1,  4, "Z 0 H -");
    case 0x1d: OP(DEC(E)              ,  1,  4, "Z 1 H -");
    case 0x1e: OP(LD(E,read8())       ,  2,  8, "- - - -");
    // case 0x1f: OP(RRA()               ,  1,  4, "0 0 0 C");
    case 0x1f: ii.RR(A); break;
    case 0x20: OP(JR(Cond::NZ,(int8_t)read8()),  2, 12/8, "- - - -");
    case 0x21: OP(LD(HL,read16())     ,  3, 12, "- - - -");
      // case 0x22: break; //(LD(Load(HL+),A)     ,  1,  8, "- - - -");
    case 0x22: ii.LD(Load(HL),A); ii.INC(HL); break;
    case 0x23: OP(INC(HL)             ,  1,  8, "- - - -");
    case 0x24: OP(INC(H)              ,  1,  4, "Z 0 H -");
    case 0x25: OP(DEC(H)              ,  1,  4, "Z 1 H -");
    case 0x26: OP(LD(H,read8())       ,  2,  8, "- - - -");
    // case 0x27: OP(DAA()               ,  1,  4, "Z - 0 C");
    case 0x28: OP(JR(Cond::Z,(int8_t)read8()),  2, 12/8, "- - - -");
    case 0x29: OP(ADD(HL,HL)          ,  1,  8, "- 0 H C");
      // case 0x2a: break; //(LD(A,Load(HL+))     ,  1,  8, "- - - -");
    case 0x2b: OP(DEC(HL)             ,  1,  8, "- - - -");
    case 0x2c: OP(INC(L)              ,  1,  4, "Z 0 H -");
    case 0x2d: OP(DEC(L)              ,  1,  4, "Z 1 H -");
    case 0x2e: OP(LD(L,read8())       ,  2,  8, "- - - -");
    // case 0x2f: OP(CPL()               ,  1,  4, "- 1 1 -");
    case 0x30: OP(JR(Cond::NC,(int8_t)read8()),  2, 12/8, "- - - -");
    case 0x31: OP(LD(SP,read16())     ,  3, 12, "- - - -");
      // case 0x32: break; //(LD(Load(HL-),A)     ,  1,  8, "- - - -");
    case 0x32: ii.LD(Load(HL),A); ii.DEC(HL); break;
    case 0x33: OP(INC(SP)             ,  1,  8, "- - - -");
    case 0x34: OP(INC(Load(HL))       ,  1, 12, "Z 0 H -");
    case 0x35: OP(DEC(Load(HL))       ,  1, 12, "Z 1 H -");
    case 0x36: OP(LD(Load(HL),read8()),  2, 12, "- - - -");
    // case 0x37: OP(SCF()               ,  1,  4, "- 0 0 1");
    case 0x38: OP(JR(Cond::C,(int8_t)read8()),  2, 12/8, "- - - -");
    case 0x39: OP(ADD(HL,SP)          ,  1,  8, "- 0 H C");
      // case 0x3a: break; //(LD(A,Load(HL-))     ,  1,  8, "- - - -");
    case 0x3b: OP(DEC(SP)             ,  1,  8, "- - - -");
    case 0x3c: OP(INC(A)              ,  1,  4, "Z 0 H -");
    case 0x3d: OP(DEC(A)              ,  1,  4, "Z 1 H -");
    case 0x3e: OP(LD(A,read8())       ,  2,  8, "- - - -");
    // case 0x3f: OP(CCF()               ,  1,  4, "- 0 0 C"); //
    case 0x40: OP(LD(B,B)             ,  1,  4, "- - - -");
    case 0x41: OP(LD(B,C)             ,  1,  4, "- - - -");
    case 0x42: OP(LD(B,D)             ,  1,  4, "- - - -");
    case 0x43: OP(LD(B,E)             ,  1,  4, "- - - -");
    case 0x44: OP(LD(B,H)             ,  1,  4, "- - - -");
    case 0x45: OP(LD(B,L)             ,  1,  4, "- - - -");
    case 0x46: OP(LD(B,Load(HL))      ,  1,  8, "- - - -");
    case 0x47: OP(LD(B,A)             ,  1,  4, "- - - -");
    case 0x48: OP(LD(C,B)             ,  1,  4, "- - - -");
    case 0x49: OP(LD(C,C)             ,  1,  4, "- - - -");
    case 0x4a: OP(LD(C,D)             ,  1,  4, "- - - -");
    case 0x4b: OP(LD(C,E)             ,  1,  4, "- - - -");
    case 0x4c: OP(LD(C,H)             ,  1,  4, "- - - -");
    case 0x4d: OP(LD(C,L)             ,  1,  4, "- - - -");
    case 0x4e: OP(LD(C,Load(HL))      ,  1,  8, "- - - -");
    case 0x4f: OP(LD(C,A)             ,  1,  4, "- - - -");
    case 0x50: OP(LD(D,B)             ,  1,  4, "- - - -");
    case 0x51: OP(LD(D,C)             ,  1,  4, "- - - -");
    case 0x52: OP(LD(D,D)             ,  1,  4, "- - - -");
    case 0x53: OP(LD(D,E)             ,  1,  4, "- - - -");
    case 0x54: OP(LD(D,H)             ,  1,  4, "- - - -");
    case 0x55: OP(LD(D,L)             ,  1,  4, "- - - -");
    case 0x56: OP(LD(D,Load(HL))      ,  1,  8, "- - - -");
    case 0x57: OP(LD(D,A)             ,  1,  4, "- - - -");
    case 0x58: OP(LD(E,B)             ,  1,  4, "- - - -");
    case 0x59: OP(LD(E,C)             ,  1,  4, "- - - -");
    case 0x5a: OP(LD(E,D)             ,  1,  4, "- - - -");
    case 0x5b: OP(LD(E,E)             ,  1,  4, "- - - -");
    case 0x5c: OP(LD(E,H)             ,  1,  4, "- - - -");
    case 0x5d: OP(LD(E,L)             ,  1,  4, "- - - -");
    case 0x5e: OP(LD(E,Load(HL))      ,  1,  8, "- - - -");
    case 0x5f: OP(LD(E,A)             ,  1,  4, "- - - -");
    case 0x60: OP(LD(H,B)             ,  1,  4, "- - - -");
    case 0x61: OP(LD(H,C)             ,  1,  4, "- - - -");
    case 0x62: OP(LD(H,D)             ,  1,  4, "- - - -");
    case 0x63: OP(LD(H,E)             ,  1,  4, "- - - -");
    case 0x64: OP(LD(H,H)             ,  1,  4, "- - - -");
    case 0x65: OP(LD(H,L)             ,  1,  4, "- - - -");
    case 0x66: OP(LD(H,Load(HL))      ,  1,  8, "- - - -");
    case 0x67: OP(LD(H,A)             ,  1,  4, "- - - -");
    case 0x68: OP(LD(L,B)             ,  1,  4, "- - - -");
    case 0x69: OP(LD(L,C)             ,  1,  4, "- - - -");
    case 0x6a: OP(LD(L,D)             ,  1,  4, "- - - -");
    case 0x6b: OP(LD(L,E)             ,  1,  4, "- - - -");
    case 0x6c: OP(LD(L,H)             ,  1,  4, "- - - -");
    case 0x6d: OP(LD(L,L)             ,  1,  4, "- - - -");
    case 0x6e: OP(LD(L,Load(HL))      ,  1,  8, "- - - -");
    case 0x6f: OP(LD(L,A)             ,  1,  4, "- - - -");
    case 0x70: OP(LD(Load(HL),B)      ,  1,  8, "- - - -");
    case 0x71: OP(LD(Load(HL),C)      ,  1,  8, "- - - -");
    case 0x72: OP(LD(Load(HL),D)      ,  1,  8, "- - - -");
    case 0x73: OP(LD(Load(HL),E)      ,  1,  8, "- - - -");
    case 0x74: OP(LD(Load(HL),H)      ,  1,  8, "- - - -");
    case 0x75: OP(LD(Load(HL),L)      ,  1,  8, "- - - -");
    case 0x76: OP(HALT()              ,  1,  4, "- - - -");
    case 0x77: OP(LD(Load(HL),A)      ,  1,  8, "- - - -");
    case 0x78: OP(LD(A,B)             ,  1,  4, "- - - -");
    case 0x79: OP(LD(A,C)             ,  1,  4, "- - - -");
    case 0x7a: OP(LD(A,D)             ,  1,  4, "- - - -");
    case 0x7b: OP(LD(A,E)             ,  1,  4, "- - - -");
    case 0x7c: OP(LD(A,H)             ,  1,  4, "- - - -");
    case 0x7d: OP(LD(A,L)             ,  1,  4, "- - - -");
    case 0x7e: OP(LD(A,Load(HL))      ,  1,  8, "- - - -");
    case 0x7f: OP(LD(A,A)             ,  1,  4, "- - - -");
    case 0x80: OP(ADD(A,B)            ,  1,  4, "Z 0 H C");
    case 0x81: OP(ADD(A,C)            ,  1,  4, "Z 0 H C");
    case 0x82: OP(ADD(A,D)            ,  1,  4, "Z 0 H C");
    case 0x83: OP(ADD(A,E)            ,  1,  4, "Z 0 H C");
    case 0x84: OP(ADD(A,H)            ,  1,  4, "Z 0 H C");
    case 0x85: OP(ADD(A,L)            ,  1,  4, "Z 0 H C");
    case 0x86: OP(ADD(A,Load(HL))     ,  1,  8, "Z 0 H C");
    case 0x87: OP(ADD(A,A)            ,  1,  4, "Z 0 H C");
    case 0x88: OP(ADC(A,B)            ,  1,  4, "Z 0 H C");
    case 0x89: OP(ADC(A,C)            ,  1,  4, "Z 0 H C");
    case 0x8a: OP(ADC(A,D)            ,  1,  4, "Z 0 H C");
    case 0x8b: OP(ADC(A,E)            ,  1,  4, "Z 0 H C");
    case 0x8c: OP(ADC(A,H)            ,  1,  4, "Z 0 H C");
    case 0x8d: OP(ADC(A,L)            ,  1,  4, "Z 0 H C");
    case 0x8e: OP(ADC(A,Load(HL))     ,  1,  8, "Z 0 H C");
    case 0x8f: OP(ADC(A,A)            ,  1,  4, "Z 0 H C");
    case 0x90: OP(SUB(B)              ,  1,  4, "Z 1 H C");
    case 0x91: OP(SUB(C)              ,  1,  4, "Z 1 H C");
    case 0x92: OP(SUB(D)              ,  1,  4, "Z 1 H C");
    case 0x93: OP(SUB(E)              ,  1,  4, "Z 1 H C");
    case 0x94: OP(SUB(H)              ,  1,  4, "Z 1 H C");
    case 0x95: OP(SUB(L)              ,  1,  4, "Z 1 H C");
    case 0x96: OP(SUB(Load(HL))       ,  1,  8, "Z 1 H C");
    case 0x97: OP(SUB(A)              ,  1,  4, "Z 1 H C");
    case 0x98: OP(SBC(B)            ,  1,  4, "Z 1 H C");
    case 0x99: OP(SBC(C)            ,  1,  4, "Z 1 H C");
    case 0x9a: OP(SBC(D)            ,  1,  4, "Z 1 H C");
    case 0x9b: OP(SBC(E)            ,  1,  4, "Z 1 H C");
    case 0x9c: OP(SBC(H)            ,  1,  4, "Z 1 H C");
    case 0x9d: OP(SBC(L)            ,  1,  4, "Z 1 H C");
    case 0x9e: OP(SBC(Load(HL))     ,  1,  8, "Z 1 H C");
    case 0x9f: OP(SBC(A)            ,  1,  4, "Z 1 H C");
    // case 0xa0: OP(AND(B)              ,  1,  4, "Z 0 1 0");
    // case 0xa1: OP(AND(C)              ,  1,  4, "Z 0 1 0");
    // case 0xa2: OP(AND(D)              ,  1,  4, "Z 0 1 0");
    // case 0xa3: OP(AND(E)              ,  1,  4, "Z 0 1 0");
    // case 0xa4: OP(AND(H)              ,  1,  4, "Z 0 1 0");
    // case 0xa5: OP(AND(L)              ,  1,  4, "Z 0 1 0");
    // case 0xa6: OP(AND(Load(HL))       ,  1,  8, "Z 0 1 0");
    // case 0xa7: OP(AND(A)              ,  1,  4, "Z 0 1 0");
    case 0xa8: OP(XOR(B)              ,  1,  4, "Z 0 0 0");
    case 0xa9: OP(XOR(C)              ,  1,  4, "Z 0 0 0");
    case 0xaa: OP(XOR(D)              ,  1,  4, "Z 0 0 0");
    case 0xab: OP(XOR(E)              ,  1,  4, "Z 0 0 0");
    case 0xac: OP(XOR(H)              ,  1,  4, "Z 0 0 0");
    case 0xad: OP(XOR(L)              ,  1,  4, "Z 0 0 0");
    case 0xae: OP(XOR(Load(HL))       ,  1,  8, "Z 0 0 0");
    case 0xaf: OP(XOR(A)              ,  1,  4, "Z 0 0 0");
    // case 0xb0: OP(OR(B)               ,  1,  4, "Z 0 0 0");
    // case 0xb1: OP(OR(C)               ,  1,  4, "Z 0 0 0");
    // case 0xb2: OP(OR(D)               ,  1,  4, "Z 0 0 0");
    // case 0xb3: OP(OR(E)               ,  1,  4, "Z 0 0 0");
    // case 0xb4: OP(OR(H)               ,  1,  4, "Z 0 0 0");
    // case 0xb5: OP(OR(L)               ,  1,  4, "Z 0 0 0");
    // case 0xb6: OP(OR(Load(HL))        ,  1,  8, "Z 0 0 0");
    // case 0xb7: OP(OR(A)               ,  1,  4, "Z 0 0 0");
    case 0xb8: OP(CP(B)               ,  1,  4, "Z 1 H C");
    case 0xb9: OP(CP(C)               ,  1,  4, "Z 1 H C");
    case 0xba: OP(CP(D)               ,  1,  4, "Z 1 H C");
    case 0xbb: OP(CP(E)               ,  1,  4, "Z 1 H C");
    case 0xbc: OP(CP(H)               ,  1,  4, "Z 1 H C");
    case 0xbd: OP(CP(L)               ,  1,  4, "Z 1 H C");
    case 0xbe: OP(CP(Load(HL))        ,  1,  8, "Z 1 H C");
    case 0xbf: OP(CP(A)               ,  1,  4, "Z 1 H C");
    case 0xc0: OP(RET(Cond::NZ)       ,  1, 20/8, "- - - -");
    case 0xc1: OP(POP(BC)             ,  1, 12, "- - - -");
    case 0xc2: OP(JP(Cond::NZ,read16()),  3, 16/12, "- - - -");
    case 0xc3: OP(JP(Cond::ALWAYS,read16()),  3, 16, "- - - -");
    case 0xc4: OP(CALL(Cond::NZ,read16()),  3, 24/12, "- - - -");
    case 0xc5: OP(PUSH(BC)            ,  1, 16, "- - - -");
    case 0xc6: OP(ADD(A,read8())      ,  2,  8, "Z 0 H C");
    case 0xc7: break; //(RST(00H)            ,  1, 16, "- - - -");
    case 0xc8: OP(RET(Cond::Z)        ,  1, 20/8, "- - - -");
    case 0xc9: OP(RET(Cond::ALWAYS)   ,  1, 16, "- - - -");
    case 0xca: OP(JP(Cond::Z,read16()),  3, 16/12, "- - - -");
    case 0xcb: break; //(PREFIX(CB)          ,  1,  4, "- - - -");
    case 0xcc: OP(CALL(Cond::Z,read16()),  3, 24/12, "- - - -");
    case 0xcd: OP(CALL(Cond::ALWAYS,read16()),  3, 24, "- - - -");
    case 0xce: OP(ADC(A, read8())      ,  2,  8, "Z 0 H C");
    case 0xcf: break; //(RST(08H)            ,  1, 16, "- - - -");
    case 0xd0: OP(RET(Cond::NC)       ,  1, 20/8, "- - - -");
    case 0xd1: OP(POP(DE)             ,  1, 12, "- - - -");
    case 0xd2: OP(JP(Cond::NC,read16()),  3, 16/12, "- - - -");
    case 0xd3: break;
    case 0xd4: OP(CALL(Cond::NC,read16()),  3, 24/12, "- - - -");
    case 0xd5: OP(PUSH(DE)            ,  1, 16, "- - - -");
    case 0xd6: OP(SUB(read8())        ,  2,  8, "Z 1 H C");
    case 0xd7: break; //(RST(10H)            ,  1, 16, "- - - -");
    case 0xd8: OP(RET(Cond::C)        ,  1, 20/8, "- - - -");
    // case 0xd9: OP(RETI()              ,  1, 16, "- - - -");
    case 0xda: OP(JP(Cond::C,read16()),  3, 16/12, "- - - -");
    case 0xdb: break;
    case 0xdc: OP(CALL(Cond::C,read16()),  3, 24/12, "- - - -");
    case 0xdd: break;
    case 0xde: OP(SBC(read8())      ,  2,  8, "Z 1 H C");
    case 0xdf: break; //(RST(18H)            ,  1, 16, "- - - -");
      // case 0xe0: OP(LDH(Load(a8),A)     ,  2, 12, "- - - -");
    case 0xe0: ii.LD(IO(read8()), A); break;
    case 0xe1: OP(POP(HL)             ,  1, 12, "- - - -");
    // case 0xe2: OP(LD(Load(C),A)       ,  2,  8, "- - - -");
    case 0xe2: ii.LD(IO(C),A); break;
    case 0xe3: break;
    case 0xe4: break;
    case 0xe5: OP(PUSH(HL)            ,  1, 16, "- - - -");
    // case 0xe6: OP(AND(read8())        ,  2,  8, "Z 0 1 0");
    case 0xe7: break; //(RST(20H)            ,  1, 16, "- - - -");
    case 0xe8: OP(ADD(SP,(int8_t)read8()),  2, 16, "0 0 H C");
    // case 0xe9: OP(JP(Cond::ALWAYS,Load(HL)),  1,  4, "- - - -");
    case 0xea: OP(LD(Load(read16()),A),  3, 16, "- - - -");
    case 0xeb: break;
    case 0xec: break;
    case 0xed: break;
    case 0xee: OP(XOR(read8())        ,  2,  8, "Z 0 0 0");
    case 0xef: break; //(RST(28H)            ,  1, 16, "- - - -");
      // case 0xf0: OP(LDH(A,Load(a8))     ,  2, 12, "- - - -");
    case 0xf0: ii.LD(A,IO(read8())); break;
    case 0xf1: OP(POP(AF)             ,  1, 12, "Z N H C");
      // case 0xf2: OP(LD(A,Load(C))       ,  2,  8, "- - - -");
    // case 0xf3: OP(DI()                ,  1,  4, "- - - -");
    case 0xf4: break;
    case 0xf5: OP(PUSH(AF)            ,  1, 16, "- - - -");
      //case 0xf6: OP(OR(read8())         ,  2,  8, "Z 0 0 0");
    case 0xf7: break; //(RST(30H)            ,  1, 16, "- - - -");
    case 0xf8: break; //(LD(HL,SP+r8)        ,  2, 12, "0 0 H C");
    case 0xf9: OP(LD(SP,HL)           ,  1,  8, "- - - -");
    case 0xfa: OP(LD(A,Load(read16())),  3, 16, "- - - -");
      // case 0xfb: OP(EI()                ,  1,  4, "- - - -");
    case 0xfc: break;
    case 0xfd: break;
    case 0xfe: OP(CP(read8())         ,  2,  8, "Z 1 H C");
    case 0xff: break; //(RST(38H)            ,  1, 16, "- - - -");
    case 0x100: OP(RLC(B)              ,  2,  8, "Z 0 0 C");
    case 0x101: OP(RLC(C)              ,  2,  8, "Z 0 0 C");
    case 0x102: OP(RLC(D)              ,  2,  8, "Z 0 0 C");
    case 0x103: OP(RLC(E)              ,  2,  8, "Z 0 0 C");
    case 0x104: OP(RLC(H)              ,  2,  8, "Z 0 0 C");
    case 0x105: OP(RLC(L)              ,  2,  8, "Z 0 0 C");
    case 0x106: OP(RLC(Load(HL))       ,  2, 16, "Z 0 0 C");
    case 0x107: OP(RLC(A)              ,  2,  8, "Z 0 0 C");
    case 0x108: OP(RRC(B)              ,  2,  8, "Z 0 0 C");
    case 0x109: OP(RRC(C)              ,  2,  8, "Z 0 0 C");
    case 0x10a: OP(RRC(D)              ,  2,  8, "Z 0 0 C");
    case 0x10b: OP(RRC(E)              ,  2,  8, "Z 0 0 C");
    case 0x10c: OP(RRC(H)              ,  2,  8, "Z 0 0 C");
    case 0x10d: OP(RRC(L)              ,  2,  8, "Z 0 0 C");
    case 0x10e: OP(RRC(Load(HL))       ,  2, 16, "Z 0 0 C");
    case 0x10f: OP(RRC(A)              ,  2,  8, "Z 0 0 C");
    case 0x110: OP(RL(B)               ,  2,  8, "Z 0 0 C");
    case 0x111: OP(RL(C)               ,  2,  8, "Z 0 0 C");
    case 0x112: OP(RL(D)               ,  2,  8, "Z 0 0 C");
    case 0x113: OP(RL(E)               ,  2,  8, "Z 0 0 C");
    case 0x114: OP(RL(H)               ,  2,  8, "Z 0 0 C");
    case 0x115: OP(RL(L)               ,  2,  8, "Z 0 0 C");
    case 0x116: OP(RL(Load(HL))        ,  2, 16, "Z 0 0 C");
    case 0x117: OP(RL(A)               ,  2,  8, "Z 0 0 C");
    case 0x118: OP(RR(B)               ,  2,  8, "Z 0 0 C");
    case 0x119: OP(RR(C)               ,  2,  8, "Z 0 0 C");
    case 0x11a: OP(RR(D)               ,  2,  8, "Z 0 0 C");
    case 0x11b: OP(RR(E)               ,  2,  8, "Z 0 0 C");
    case 0x11c: OP(RR(H)               ,  2,  8, "Z 0 0 C");
    case 0x11d: OP(RR(L)               ,  2,  8, "Z 0 0 C");
    case 0x11e: OP(RR(Load(HL))        ,  2, 16, "Z 0 0 C");
    case 0x11f: OP(RR(A)               ,  2,  8, "Z 0 0 C");
    case 0x120: OP(SLA(B)              ,  2,  8, "Z 0 0 C");
    case 0x121: OP(SLA(C)              ,  2,  8, "Z 0 0 C");
    case 0x122: OP(SLA(D)              ,  2,  8, "Z 0 0 C");
    case 0x123: OP(SLA(E)              ,  2,  8, "Z 0 0 C");
    case 0x124: OP(SLA(H)              ,  2,  8, "Z 0 0 C");
    case 0x125: OP(SLA(L)              ,  2,  8, "Z 0 0 C");
    case 0x126: OP(SLA(Load(HL))       ,  2, 16, "Z 0 0 C");
    case 0x127: OP(SLA(A)              ,  2,  8, "Z 0 0 C");
    case 0x128: OP(SRA(B)              ,  2,  8, "Z 0 0 0");
    case 0x129: OP(SRA(C)              ,  2,  8, "Z 0 0 0");
    case 0x12a: OP(SRA(D)              ,  2,  8, "Z 0 0 0");
    case 0x12b: OP(SRA(E)              ,  2,  8, "Z 0 0 0");
    case 0x12c: OP(SRA(H)              ,  2,  8, "Z 0 0 0");
    case 0x12d: OP(SRA(L)              ,  2,  8, "Z 0 0 0");
    case 0x12e: OP(SRA(Load(HL))       ,  2, 16, "Z 0 0 0");
    case 0x12f: OP(SRA(A)              ,  2,  8, "Z 0 0 0");
    // case 0x130: OP(SWAP(B)             ,  2,  8, "Z 0 0 0");
    // case 0x131: OP(SWAP(C)             ,  2,  8, "Z 0 0 0");
    // case 0x132: OP(SWAP(D)             ,  2,  8, "Z 0 0 0");
    // case 0x133: OP(SWAP(E)             ,  2,  8, "Z 0 0 0");
    // case 0x134: OP(SWAP(H)             ,  2,  8, "Z 0 0 0");
    // case 0x135: OP(SWAP(L)             ,  2,  8, "Z 0 0 0");
    // case 0x136: OP(SWAP(Load(HL))      ,  2, 16, "Z 0 0 0");
    // case 0x137: OP(SWAP(A)             ,  2,  8, "Z 0 0 0");
    case 0x138: OP(SRL(B)              ,  2,  8, "Z 0 0 C");
    case 0x139: OP(SRL(C)              ,  2,  8, "Z 0 0 C");
    case 0x13a: OP(SRL(D)              ,  2,  8, "Z 0 0 C");
    case 0x13b: OP(SRL(E)              ,  2,  8, "Z 0 0 C");
    case 0x13c: OP(SRL(H)              ,  2,  8, "Z 0 0 C");
    case 0x13d: OP(SRL(L)              ,  2,  8, "Z 0 0 C");
    case 0x13e: OP(SRL(Load(HL))       ,  2, 16, "Z 0 0 C");
    case 0x13f: OP(SRL(A)              ,  2,  8, "Z 0 0 C");
    case 0x140: OP(BIT(0,B)            ,  2,  8, "Z 0 1 -");
    case 0x141: OP(BIT(0,C)            ,  2,  8, "Z 0 1 -");
    case 0x142: OP(BIT(0,D)            ,  2,  8, "Z 0 1 -");
    case 0x143: OP(BIT(0,E)            ,  2,  8, "Z 0 1 -");
    case 0x144: OP(BIT(0,H)            ,  2,  8, "Z 0 1 -");
    case 0x145: OP(BIT(0,L)            ,  2,  8, "Z 0 1 -");
    case 0x146: OP(BIT(0,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x147: OP(BIT(0,A)            ,  2,  8, "Z 0 1 -");
    case 0x148: OP(BIT(1,B)            ,  2,  8, "Z 0 1 -");
    case 0x149: OP(BIT(1,C)            ,  2,  8, "Z 0 1 -");
    case 0x14a: OP(BIT(1,D)            ,  2,  8, "Z 0 1 -");
    case 0x14b: OP(BIT(1,E)            ,  2,  8, "Z 0 1 -");
    case 0x14c: OP(BIT(1,H)            ,  2,  8, "Z 0 1 -");
    case 0x14d: OP(BIT(1,L)            ,  2,  8, "Z 0 1 -");
    case 0x14e: OP(BIT(1,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x14f: OP(BIT(1,A)            ,  2,  8, "Z 0 1 -");
    case 0x150: OP(BIT(2,B)            ,  2,  8, "Z 0 1 -");
    case 0x151: OP(BIT(2,C)            ,  2,  8, "Z 0 1 -");
    case 0x152: OP(BIT(2,D)            ,  2,  8, "Z 0 1 -");
    case 0x153: OP(BIT(2,E)            ,  2,  8, "Z 0 1 -");
    case 0x154: OP(BIT(2,H)            ,  2,  8, "Z 0 1 -");
    case 0x155: OP(BIT(2,L)            ,  2,  8, "Z 0 1 -");
    case 0x156: OP(BIT(2,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x157: OP(BIT(2,A)            ,  2,  8, "Z 0 1 -");
    case 0x158: OP(BIT(3,B)            ,  2,  8, "Z 0 1 -");
    case 0x159: OP(BIT(3,C)            ,  2,  8, "Z 0 1 -");
    case 0x15a: OP(BIT(3,D)            ,  2,  8, "Z 0 1 -");
    case 0x15b: OP(BIT(3,E)            ,  2,  8, "Z 0 1 -");
    case 0x15c: OP(BIT(3,H)            ,  2,  8, "Z 0 1 -");
    case 0x15d: OP(BIT(3,L)            ,  2,  8, "Z 0 1 -");
    case 0x15e: OP(BIT(3,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x15f: OP(BIT(3,A)            ,  2,  8, "Z 0 1 -");
    case 0x160: OP(BIT(4,B)            ,  2,  8, "Z 0 1 -");
    case 0x161: OP(BIT(4,C)            ,  2,  8, "Z 0 1 -");
    case 0x162: OP(BIT(4,D)            ,  2,  8, "Z 0 1 -");
    case 0x163: OP(BIT(4,E)            ,  2,  8, "Z 0 1 -");
    case 0x164: OP(BIT(4,H)            ,  2,  8, "Z 0 1 -");
    case 0x165: OP(BIT(4,L)            ,  2,  8, "Z 0 1 -");
    case 0x166: OP(BIT(4,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x167: OP(BIT(4,A)            ,  2,  8, "Z 0 1 -");
    case 0x168: OP(BIT(5,B)            ,  2,  8, "Z 0 1 -");
    case 0x169: OP(BIT(5,C)            ,  2,  8, "Z 0 1 -");
    case 0x16a: OP(BIT(5,D)            ,  2,  8, "Z 0 1 -");
    case 0x16b: OP(BIT(5,E)            ,  2,  8, "Z 0 1 -");
    case 0x16c: OP(BIT(5,H)            ,  2,  8, "Z 0 1 -");
    case 0x16d: OP(BIT(5,L)            ,  2,  8, "Z 0 1 -");
    case 0x16e: OP(BIT(5,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x16f: OP(BIT(5,A)            ,  2,  8, "Z 0 1 -");
    case 0x170: OP(BIT(6,B)            ,  2,  8, "Z 0 1 -");
    case 0x171: OP(BIT(6,C)            ,  2,  8, "Z 0 1 -");
    case 0x172: OP(BIT(6,D)            ,  2,  8, "Z 0 1 -");
    case 0x173: OP(BIT(6,E)            ,  2,  8, "Z 0 1 -");
    case 0x174: OP(BIT(6,H)            ,  2,  8, "Z 0 1 -");
    case 0x175: OP(BIT(6,L)            ,  2,  8, "Z 0 1 -");
    case 0x176: OP(BIT(6,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x177: OP(BIT(6,A)            ,  2,  8, "Z 0 1 -");
    case 0x178: OP(BIT(7,B)            ,  2,  8, "Z 0 1 -");
    case 0x179: OP(BIT(7,C)            ,  2,  8, "Z 0 1 -");
    case 0x17a: OP(BIT(7,D)            ,  2,  8, "Z 0 1 -");
    case 0x17b: OP(BIT(7,E)            ,  2,  8, "Z 0 1 -");
    case 0x17c: OP(BIT(7,H)            ,  2,  8, "Z 0 1 -");
    case 0x17d: OP(BIT(7,L)            ,  2,  8, "Z 0 1 -");
    case 0x17e: OP(BIT(7,Load(HL))     ,  2, 16, "Z 0 1 -");
    case 0x17f: OP(BIT(7,A)            ,  2,  8, "Z 0 1 -");
    case 0x180: OP(RES(0,B)            ,  2,  8, "- - - -");
    case 0x181: OP(RES(0,C)            ,  2,  8, "- - - -");
    case 0x182: OP(RES(0,D)            ,  2,  8, "- - - -");
    case 0x183: OP(RES(0,E)            ,  2,  8, "- - - -");
    case 0x184: OP(RES(0,H)            ,  2,  8, "- - - -");
    case 0x185: OP(RES(0,L)            ,  2,  8, "- - - -");
    case 0x186: OP(RES(0,Load(HL))     ,  2, 16, "- - - -");
    case 0x187: OP(RES(0,A)            ,  2,  8, "- - - -");
    case 0x188: OP(RES(1,B)            ,  2,  8, "- - - -");
    case 0x189: OP(RES(1,C)            ,  2,  8, "- - - -");
    case 0x18a: OP(RES(1,D)            ,  2,  8, "- - - -");
    case 0x18b: OP(RES(1,E)            ,  2,  8, "- - - -");
    case 0x18c: OP(RES(1,H)            ,  2,  8, "- - - -");
    case 0x18d: OP(RES(1,L)            ,  2,  8, "- - - -");
    case 0x18e: OP(RES(1,Load(HL))     ,  2, 16, "- - - -");
    case 0x18f: OP(RES(1,A)            ,  2,  8, "- - - -");
    case 0x190: OP(RES(2,B)            ,  2,  8, "- - - -");
    case 0x191: OP(RES(2,C)            ,  2,  8, "- - - -");
    case 0x192: OP(RES(2,D)            ,  2,  8, "- - - -");
    case 0x193: OP(RES(2,E)            ,  2,  8, "- - - -");
    case 0x194: OP(RES(2,H)            ,  2,  8, "- - - -");
    case 0x195: OP(RES(2,L)            ,  2,  8, "- - - -");
    case 0x196: OP(RES(2,Load(HL))     ,  2, 16, "- - - -");
    case 0x197: OP(RES(2,A)            ,  2,  8, "- - - -");
    case 0x198: OP(RES(3,B)            ,  2,  8, "- - - -");
    case 0x199: OP(RES(3,C)            ,  2,  8, "- - - -");
    case 0x19a: OP(RES(3,D)            ,  2,  8, "- - - -");
    case 0x19b: OP(RES(3,E)            ,  2,  8, "- - - -");
    case 0x19c: OP(RES(3,H)            ,  2,  8, "- - - -");
    case 0x19d: OP(RES(3,L)            ,  2,  8, "- - - -");
    case 0x19e: OP(RES(3,Load(HL))     ,  2, 16, "- - - -");
    case 0x19f: OP(RES(3,A)            ,  2,  8, "- - - -");
    case 0x1a0: OP(RES(4,B)            ,  2,  8, "- - - -");
    case 0x1a1: OP(RES(4,C)            ,  2,  8, "- - - -");
    case 0x1a2: OP(RES(4,D)            ,  2,  8, "- - - -");
    case 0x1a3: OP(RES(4,E)            ,  2,  8, "- - - -");
    case 0x1a4: OP(RES(4,H)            ,  2,  8, "- - - -");
    case 0x1a5: OP(RES(4,L)            ,  2,  8, "- - - -");
    case 0x1a6: OP(RES(4,Load(HL))     ,  2, 16, "- - - -");
    case 0x1a7: OP(RES(4,A)            ,  2,  8, "- - - -");
    case 0x1a8: OP(RES(5,B)            ,  2,  8, "- - - -");
    case 0x1a9: OP(RES(5,C)            ,  2,  8, "- - - -");
    case 0x1aa: OP(RES(5,D)            ,  2,  8, "- - - -");
    case 0x1ab: OP(RES(5,E)            ,  2,  8, "- - - -");
    case 0x1ac: OP(RES(5,H)            ,  2,  8, "- - - -");
    case 0x1ad: OP(RES(5,L)            ,  2,  8, "- - - -");
    case 0x1ae: OP(RES(5,Load(HL))     ,  2, 16, "- - - -");
    case 0x1af: OP(RES(5,A)            ,  2,  8, "- - - -");
    case 0x1b0: OP(RES(6,B)            ,  2,  8, "- - - -");
    case 0x1b1: OP(RES(6,C)            ,  2,  8, "- - - -");
    case 0x1b2: OP(RES(6,D)            ,  2,  8, "- - - -");
    case 0x1b3: OP(RES(6,E)            ,  2,  8, "- - - -");
    case 0x1b4: OP(RES(6,H)            ,  2,  8, "- - - -");
    case 0x1b5: OP(RES(6,L)            ,  2,  8, "- - - -");
    case 0x1b6: OP(RES(6,Load(HL))     ,  2, 16, "- - - -");
    case 0x1b7: OP(RES(6,A)            ,  2,  8, "- - - -");
    case 0x1b8: OP(RES(7,B)            ,  2,  8, "- - - -");
    case 0x1b9: OP(RES(7,C)            ,  2,  8, "- - - -");
    case 0x1ba: OP(RES(7,D)            ,  2,  8, "- - - -");
    case 0x1bb: OP(RES(7,E)            ,  2,  8, "- - - -");
    case 0x1bc: OP(RES(7,H)            ,  2,  8, "- - - -");
    case 0x1bd: OP(RES(7,L)            ,  2,  8, "- - - -");
    case 0x1be: OP(RES(7,Load(HL))     ,  2, 16, "- - - -");
    case 0x1bf: OP(RES(7,A)            ,  2,  8, "- - - -");
    case 0x1c0: OP(SET(0,B)            ,  2,  8, "- - - -");
    case 0x1c1: OP(SET(0,C)            ,  2,  8, "- - - -");
    case 0x1c2: OP(SET(0,D)            ,  2,  8, "- - - -");
    case 0x1c3: OP(SET(0,E)            ,  2,  8, "- - - -");
    case 0x1c4: OP(SET(0,H)            ,  2,  8, "- - - -");
    case 0x1c5: OP(SET(0,L)            ,  2,  8, "- - - -");
    case 0x1c6: OP(SET(0,Load(HL))     ,  2, 16, "- - - -");
    case 0x1c7: OP(SET(0,A)            ,  2,  8, "- - - -");
    case 0x1c8: OP(SET(1,B)            ,  2,  8, "- - - -");
    case 0x1c9: OP(SET(1,C)            ,  2,  8, "- - - -");
    case 0x1ca: OP(SET(1,D)            ,  2,  8, "- - - -");
    case 0x1cb: OP(SET(1,E)            ,  2,  8, "- - - -");
    case 0x1cc: OP(SET(1,H)            ,  2,  8, "- - - -");
    case 0x1cd: OP(SET(1,L)            ,  2,  8, "- - - -");
    case 0x1ce: OP(SET(1,Load(HL))     ,  2, 16, "- - - -");
    case 0x1cf: OP(SET(1,A)            ,  2,  8, "- - - -");
    case 0x1d0: OP(SET(2,B)            ,  2,  8, "- - - -");
    case 0x1d1: OP(SET(2,C)            ,  2,  8, "- - - -");
    case 0x1d2: OP(SET(2,D)            ,  2,  8, "- - - -");
    case 0x1d3: OP(SET(2,E)            ,  2,  8, "- - - -");
    case 0x1d4: OP(SET(2,H)            ,  2,  8, "- - - -");
    case 0x1d5: OP(SET(2,L)            ,  2,  8, "- - - -");
    case 0x1d6: OP(SET(2,Load(HL))     ,  2, 16, "- - - -");
    case 0x1d7: OP(SET(2,A)            ,  2,  8, "- - - -");
    case 0x1d8: OP(SET(3,B)            ,  2,  8, "- - - -");
    case 0x1d9: OP(SET(3,C)            ,  2,  8, "- - - -");
    case 0x1da: OP(SET(3,D)            ,  2,  8, "- - - -");
    case 0x1db: OP(SET(3,E)            ,  2,  8, "- - - -");
    case 0x1dc: OP(SET(3,H)            ,  2,  8, "- - - -");
    case 0x1dd: OP(SET(3,L)            ,  2,  8, "- - - -");
    case 0x1de: OP(SET(3,Load(HL))     ,  2, 16, "- - - -");
    case 0x1df: OP(SET(3,A)            ,  2,  8, "- - - -");
    case 0x1e0: OP(SET(4,B)            ,  2,  8, "- - - -");
    case 0x1e1: OP(SET(4,C)            ,  2,  8, "- - - -");
    case 0x1e2: OP(SET(4,D)            ,  2,  8, "- - - -");
    case 0x1e3: OP(SET(4,E)            ,  2,  8, "- - - -");
    case 0x1e4: OP(SET(4,H)            ,  2,  8, "- - - -");
    case 0x1e5: OP(SET(4,L)            ,  2,  8, "- - - -");
    case 0x1e6: OP(SET(4,Load(HL))     ,  2, 16, "- - - -");
    case 0x1e7: OP(SET(4,A)            ,  2,  8, "- - - -");
    case 0x1e8: OP(SET(5,B)            ,  2,  8, "- - - -");
    case 0x1e9: OP(SET(5,C)            ,  2,  8, "- - - -");
    case 0x1ea: OP(SET(5,D)            ,  2,  8, "- - - -");
    case 0x1eb: OP(SET(5,E)            ,  2,  8, "- - - -");
    case 0x1ec: OP(SET(5,H)            ,  2,  8, "- - - -");
    case 0x1ed: OP(SET(5,L)            ,  2,  8, "- - - -");
    case 0x1ee: OP(SET(5,Load(HL))     ,  2, 16, "- - - -");
    case 0x1ef: OP(SET(5,A)            ,  2,  8, "- - - -");
    case 0x1f0: OP(SET(6,B)            ,  2,  8, "- - - -");
    case 0x1f1: OP(SET(6,C)            ,  2,  8, "- - - -");
    case 0x1f2: OP(SET(6,D)            ,  2,  8, "- - - -");
    case 0x1f3: OP(SET(6,E)            ,  2,  8, "- - - -");
    case 0x1f4: OP(SET(6,H)            ,  2,  8, "- - - -");
    case 0x1f5: OP(SET(6,L)            ,  2,  8, "- - - -");
    case 0x1f6: OP(SET(6,Load(HL))     ,  2, 16, "- - - -");
    case 0x1f7: OP(SET(6,A)            ,  2,  8, "- - - -");
    case 0x1f8: OP(SET(7,B)            ,  2,  8, "- - - -");
    case 0x1f9: OP(SET(7,C)            ,  2,  8, "- - - -");
    case 0x1fa: OP(SET(7,D)            ,  2,  8, "- - - -");
    case 0x1fb: OP(SET(7,E)            ,  2,  8, "- - - -");
    case 0x1fc: OP(SET(7,H)            ,  2,  8, "- - - -");
    case 0x1fd: OP(SET(7,L)            ,  2,  8, "- - - -");
    case 0x1fe: OP(SET(7,Load(HL))     ,  2, 16, "- - - -");
    case 0x1ff: OP(SET(7,A)            ,  2,  8, "- - - -");
    default:
      printf("unhandled op %x\n", op);
      break;
    }
  }
  u8 read8() { return buf[registers.PC++]; }
  u16 read16() { registers.PC += 2; return buf[registers.PC - 2] + buf[registers.PC - 1] * 256; }
};

#include "gpu.cc"

u8 *full_screen_buf; // 256x256 pixels;
u8 *display_buf; // 160x144 pixels;
u8 *io_buf;

int main() {
  char stdout_buf[2048];
  setvbuf(stdout, stdout_buf, _IOFBF, sizeof stdout_buf);
  Registers reg;
  Memory memory;
  Executor exec(reg, memory);
  OpPrint printer(reg);
  struct PPU ppu { memory };
  OpParser pp(reg, memory, exec, fopen("data/DMG_ROM.bin", "r"));

  while(reg.PC < 0x100) {
    for(int i=0; i<1000; i++) {
      pp.Step();
      ppu.Step(400);
      sleep(0);
    }
  }
  reg.dump();
}
