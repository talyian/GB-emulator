#include "emulator.hpp"
#include "data/dmg_boot.hpp"
#include "data/bgbtest.hpp"
#include "data/04-op r,imm.gb.hpp"
#include "data/05-op rp.gb.hpp"
#include "data/06-ld r,r.gb.hpp"

emulator_t::emulator_t() : mmu(rom, ram) {
  mmu.bios_rom = DMG_ROM_bin;
  // u8 * _gb = ___data_bgbtest_gb;
  // int _gb_len = ___data_bgbtest_gb_len;
  u8 * _gb = ______data_cpu_instrs_individual_06_ld_r_r_gb;
  int _gb_len = ______data_cpu_instrs_individual_06_ld_r_r_gb_len;
  mmu.load_cart(_gb, _gb_len);
  
  decoder.mmu = &mmu;
  decoder.ii.mmu = &mmu;
  ppu.memory = &mmu;
  printer.mmu = &mmu;
}

u32 emulator_t::single_step() {
  u8 interrupt = (mmu.get(0xFFFF) & mmu.get(0xFF0F));
  if (cpu.IME && interrupt) {
    // when an interrupt triggers we clear the IME
    // and the flag that triggered
    // and restart from a halting state
    cpu.IME = 0;
    cpu.halted = false;
    if (interrupt == 1) {
      mmu.get_ref(0xFF0F) &= ~0x1;
      decoder.ii._push(decoder.pc);
      decoder.pc = 0x40;
    } else {
      log("interrupt", interrupt);
    }
    // TODO: does this actually take zero time?    
    return 0;
  }
  if (decoder.error) { log(decoder.pc_start, "decoder error"); _stop(); }
  if (decoder.ii.error) { log(decoder.pc_start, "runner error"); _stop(); }

  if (!cpu.halted) {
    decoder.decode();
  }

  if (mmu.get(0xFF46)) {
    // technically this won't work if we transfer from 0
    // but that seems highly unlikely
    mmu.set(0xFF46, 0);
    dma_transfer(&mmu, mmu.get(0xFF46));
  }
  
  u32 dt = 16;              // TODO: do timing based on actual instruction decodetime
  dt = 8;
  ppu.tick(dt);
  return dt;
}
void emulator_t::step(i32 ticks) {
  // _runner.verbose_log = true;
  while(ticks > 0) {
    // debug.step();
    // if (debug.is_debugging) {
    //   printer.pc = decoder.pc;
    //   decoder.ii.dump();
    //   _log(decoder.pc);
    //   printer.decode();
    //   break;
    // }
    ticks -= single_step();
    // if (debug.is_stepping) { debug.is_stepping = false; debug.is_debugging = true; } 
  }
}
