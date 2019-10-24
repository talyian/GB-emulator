// single compilation unit for lib_gb
#include "graphics.cpp"
#include "executor.cpp"
#include "emulator.cpp"

#include "lib_gb.hpp"

extern "C" {
  typedef void * Emulator;
  Emulator EXPORT spgb_create_emulator() {
    return new emulator_t();
  }
  void EXPORT spgb_load_cart(Emulator emu, u8 * bytes, u32 len) {
    ((emulator_t*)emu)->load_cart(bytes, len);
  }
  void EXPORT spgb_step(Emulator emu, u32 ticks) {
    ((emulator_t*)emu)->step(ticks);
  }
  void EXPORT spgb_step_instruction(Emulator emu) {
    ((emulator_t*)emu)->single_step();
  }
  void EXPORT spgb_step_frame(Emulator emu);
  void EXPORT spgb_button_down(Emulator emu, Buttons button) {
    ((emulator_t*)emu)->joypad.button_down(button);
  }
  void EXPORT spgb_button_up(Emulator emu, Buttons button) {
    ((emulator_t*)emu)->joypad.button_up(button);
  }
}

namespace logs {
  void _log(CPU::Reg16 v) { spgb_logx16((u16)v); };
  void _log(str s) { spgb_logs((const char*)s.data, s.size); }
  void _log(u8 v) { spgb_logx8(v); }
  void _log(u16 v) { spgb_logx16(v); }
  void _log(u32 v) { spgb_logx32(v); }
  void _log(i32 v) { spgb_logf(v); }
  void _log(double f) { spgb_logf(f); }
  void _log(const char* s) { _log(str{s}); }
  void _log(void* s) { spgb_logp(s); }
}