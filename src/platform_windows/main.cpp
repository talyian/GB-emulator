#include "base.hpp"
#include "emulator.hpp"
#include "platform.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>

#include <gl/gl.h>
#include "win32_opengl.hpp"

// #define NOSWAP

extern "C" size_t sslen(const char* s) { return strlen(s); }

// Platform API: these functions are declared in platform.hpp and must
// be provided by the platform.
extern "C" {
  void _logf(double v) { printf("%f ", v); }
  void _logx8(u8 v) { printf("%02x ", v); }
  void _logx16(u16 v) { printf("%04x ", v); }
  void _logx32(u32 v) { printf("%04x ", v); }
  void _logs(const char* s, u32 len) { printf("%.*s ", len, s); }
  void _showlog() { printf("\n"); }
  void _stop() { exit(1); }
  void _logp(void* v) { printf("%p ", v); }
  void _serial_putc(u8 v) {
    static u64 sequence = 0;
    static char line[20];
    static u32 p = 0;
    auto flush = [&]() { printf("<<< %.*s\n", p, line); p = 0; };
    if (v == '\n' || p == 20) { flush(); } else { line[p++] = v; }
    sequence = sequence * 0x100 + v;
    if ((sequence & 0xFFFFFFFFFFFF) == *(u64 *)"dessaP\0") {
      flush();
      exit(0);
    }
    if ((sequence & 0xFFFFFFFFFFFF) == *(u64 *)"deliaF\0") {
      flush();
      exit(-1);
    }
  }
}

// a global variable to hold current window/opengl/emulator context.
// Not hygienic, but it's an easy way to get access to these fields
// from inside WndProc or _push_frame.
struct Win32Emulator {
  HWND hwnd = 0;
  HGLRC gl = 0;
  HDC hdc = 0;
  emulator_t emu;
  u32 display_texture = 0;
} win32_emulator;

// Main Window event handler.
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_QUIT:
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_SIZE: {
    u16 w = lParam;
    u16 h = lParam >> 16;
    glViewport(0, 0, w, h);
    break;
  }
  case WM_KEYUP: {
    auto* jp = &win32_emulator.emu.joypad;
    switch (wParam) {
    case VK_RETURN: jp->button_up(Buttons::START); return 0;
    case VK_SHIFT:
    case VK_RSHIFT: jp->button_up(Buttons::SELECT); return 0;
    case 'J': jp->button_up(Buttons::B); return 0;
    case 'K': jp->button_up(Buttons::A); return 0;
    case 'W': jp->button_up(Buttons::UP); return 0;
    case 'S': jp->button_up(Buttons::DOWN); return 0;
    case 'A': jp->button_up(Buttons::LEFT); return 0;
    case 'D': jp->button_up(Buttons::RIGHT); return 0;
    case VK_ESCAPE:
      DestroyWindow(hwnd); return 0;
    }
  }
  case WM_KEYDOWN: {
    auto* jp = &win32_emulator.emu.joypad;
    switch (wParam) {
    case VK_RETURN: jp->button_down(Buttons::START); return 0;
    case VK_SHIFT:
    case VK_RSHIFT: jp->button_down(Buttons::SELECT); return 0;
    case 'J': jp->button_down(Buttons::B); return 0;
    case 'K': jp->button_down(Buttons::A); return 0;
    case 'W': jp->button_down(Buttons::UP); return 0;
    case 'S': jp->button_down(Buttons::DOWN); return 0;
    case 'A': jp->button_down(Buttons::LEFT); return 0;
    case 'D': jp->button_down(Buttons::RIGHT); return 0;
    case VK_ESCAPE:
      DestroyWindow(hwnd); return 0;
    }
  }
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}

int main(int argc, char** argv) {
  emulator_t& emu = win32_emulator.emu;

  // Select Cart from argv or open file Dialog box
  auto load_cart_file = [] (char* path, emulator_t* emu) -> void {
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "%s: file not found\n", path); exit(19); }
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, 0);
    u8* buf = new u8[len];
    fread(buf, 1, len, f);
    fclose(f);
    emu->load_cart(buf, len);
  };
  if (argc > 1) {
    load_cart_file(argv[1], &emu);
  }
  else {
    char filename[0x100] = { 0 };
    OPENFILENAME ofn {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrInitialDir = NULL;
    if (!GetOpenFileName(&ofn)) {
      printf("no file specified\n");
      return 18;
    }
    load_cart_file(ofn.lpstrFile, &emu);
  }

  // init win32 window
  WNDCLASS wnd_class{ 0 };
  wnd_class.hInstance = NULL;
  wnd_class.lpfnWndProc = WndProc;
  wnd_class.lpszClassName = "gbo";
  wnd_class.style = CS_HREDRAW | CS_VREDRAW;
  wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
  if (!RegisterClass(&wnd_class)) {
    MessageBox(NULL, "RegisterClass failed", "Error", MB_OK);
    return 101;
  }

  RECT r = RECT();
  r.top = 0; r.left = 0;
  r.bottom = 144 * 2; r.right = 160 * 2;
  AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
  HWND hwnd = win32_emulator.hwnd = CreateWindow(
    wnd_class.lpszClassName,
    "hi",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    r.right - r.left,
    r.bottom - r.top, NULL, NULL, NULL, NULL);
  ShowWindow(hwnd, SW_RESTORE);

  MSG msg;

  // init WGL -- set hdc pixel format, then use wgl to create GL
  // context. wglCreateContext will fail without a compatible pixel
  // format.
  HDC hdc = win32_emulator.hdc = GetDC(hwnd);
  PIXELFORMATDESCRIPTOR pfd;
  memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  pfd.iLayerType = PFD_MAIN_PLANE;
  int px_format = ChoosePixelFormat(hdc, &pfd);
  if (px_format == 0) {
    printf("ChoosePixelFormat error\n");
    return __LINE__;
  }
  if (!SetPixelFormat(hdc, px_format, &pfd)) {
    printf("SetPixelFormat error\n");
    return __LINE__;
  }

  HGLRC gl = win32_emulator.gl = wglCreateContext(hdc);
  wglMakeCurrent(hdc, gl);
  auto version = glGetString(GL_VERSION);
  printf("OpenGL Version: %s\n", version);
  if (version[0] < '3') {
    printf("OpenGL Version Too Low\n");
    return 1;
  }

  glf::load_functions();
  glEnable(glf::GL_DEBUG_OUTPUT);
  glf::DebugMessageCallback(MessageCallback, nullptr);
  u32 display;

  auto program = glf::glCreateProgram();
  auto vertex_shader = glf::glCreateShader(glf::VERTEX_SHADER);
  const char * vs_source = R"STR(
#version 110
varying vec3 pos;
varying vec2 uv;
void main() { 
  pos = gl_Vertex.xyz;
  uv = gl_MultiTexCoord0.xy;
  gl_Position = 0.5 * gl_Vertex;
}
)STR";
  glf::ShaderSource(vertex_shader, 1, &vs_source, 0);
  auto fragment_shader = glf::CreateShader(glf::FRAGMENT_SHADER);
  const char * fs_source = R"STR(
#version 110
varying vec3 pos;
varying vec2 uv;
uniform sampler2D tx_screen;
// Translate 8-bit 216-color-cube to RGB
void main() { 
  float vv = texture2D(tx_screen, uv).r;
  float vf = vv * 255.0 / 216.0;
  float r = floor(vf * 6.0);
  float g = floor(vf * 36.0 - (r) * 6.0);
  float b = vf * 216.0 - (r) * 36.0 - (g) * 6.0;
  gl_FragColor = vec4(r / 5.0, g / 5.0, b / 5.0, 1.0);
}
)STR";
  glf::ShaderSource(fragment_shader, 1, &fs_source, 0);
  glf::AttachShader(program, vertex_shader);
  glf::CompileShader(fragment_shader);
  auto get_log = [] (glf::glShader& shader) -> void { 
    GLint info_log_size = 0;
    glf::GetShaderiv(shader, glf::INFO_LOG_LENGTH, &info_log_size);
    char * info_log = new char[info_log_size];
    glf::GetShaderInfoLog(shader, info_log_size, nullptr, info_log);
    printf("Shader Info Log: '%.*s'\n", info_log_size, info_log);
    delete [] info_log;
  };
  get_log(fragment_shader);
  glf::AttachShader(program, fragment_shader);
  glf::CompileShader(vertex_shader);
  get_log(vertex_shader);
  glf::LinkProgram(program);
  glf::UseProgram(program);
  {
    char * info_log = new char[1024];
    glf::GetProgramInfoLog(program, 1024, nullptr, info_log);
    printf("Vert Shader Info Log: '%s'\n", info_log);
  }
  #ifdef NOSWAP
  glDrawBuffer(GL_FRONT);
  #endif
  GLuint vbo = 0;
  {
    f32 w = 1.0f;
    struct Vertex { f32 x, y, z; f32 u, v; } vertices[] = {
      {-w, -w, 0, 0, 1},
      {w, -w, 0,  1, 1},
      {w, w, 0,   1, 0},
      {-w, -w, 0, 0, 1},
      {w, w, 0,   1, 0},
      {-w, w, 0,  0, 0}
    };
    glf::GenBuffers(1, &vbo);
    glf::BindBuffer(glf::ARRAY_BUFFER, vbo);
    glf::BufferData(
      glf::ARRAY_BUFFER,
      sizeof(vertices),
      (const void*)vertices,
      glf::DrawType::STATIC_DRAW);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(vertices[0]), 0);
    glTexCoordPointer(2, GL_FLOAT, sizeof(vertices[0]), (void*)12);
  }
  printf("vs: %d, fs: %d, error: %d\n", vertex_shader.id, fragment_shader.id, glGetError());
  
  glGenTextures(1, &display);
  glBindTexture(GL_TEXTURE_2D, display);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  win32_emulator.display_texture = display;

  char line[64] {0};
  // emu.debug.name_function("main", 0xC300, 0xc315);
  // emu.debug.name_function("test_timer", 0xC318, 0xc345);
  if (argc > 1 && strstr(argv[1], "instr_timing"))
    emu.debug.set_breakpoint(0xC300);
  if (argc > 1 && strstr(argv[1], "bgbtest")) {
    emu.debug.set_breakpoint(0x40); // vblank
    // emu.debug.set_breakpoint(0x40); // vblank
    // emu.debug.set_breakpoint(0x48); // lcdc
    // emu.debug.set_breakpoint(0x150); // entry
    // emu.debug.set_breakpoint(0x416);
    // emu.debug.set_breakpoint(0x1e7); // vblank
    // emu.debug.set_breakpoint(0x1e7); // vblank
    // emu.debug.set_breakpoint(0x1e1); // halt loop
    // emu.debug.set_breakpoint(0x219); // badfunction ?
  }
  while (true) {
    if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) exit(0);
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      continue;
    }

    emu.debug.step();
    
    if (emu.debug.state.type == Debugger::State::PAUSE) {
      log("\x1b[1;31mime\x1b[0m", (u8)emu.cpu.IME, "interrupt", emu.mmu.get(0xFFFF), emu.mmu.get(0xFF0F));
      printf("Timer: CTL=%02x DIV=%02x TIMA=%02x, ticks=%d\n",
             emu.timer.Control, emu.timer.DIV, emu.timer.TIMA, (u16)emu.timer.monotonic_t);
      printf("PPU: STAT=%02x, LCDC=%02x LY=%02x LYC=%02x [%d%d%d]\n",
             emu.ppu.LcdStatus.v, emu.ppu.LcdControl,
             emu.ppu.LineY, emu.ppu.LineYMark,
             emu.ppu.LcdStatusMatch, emu.ppu.LcdStatusLastMatch, 
             emu.ppu.LcdStatusMatch - emu.ppu.LcdStatusLastMatch);
      auto rr = emu.cpu.registers;
      printf("AF   BC   DE   HL   SP   PC\n");
      printf("%04x %04x %04x %04x %04x %04x\n",
             (u16)rr.AF, (u16)rr.BC, (u16)rr.DE, (u16)rr.HL, (u16)rr.SP,
             emu._dasher.PC);
      _log("\x1b[1;32m                  ");
      emu._printer.PC = emu._dasher.PC; emu._printer.decode();
      printf("\x1b[0m DEBUG %04x> ", emu._dasher.PC);

      fgets(line, 63, stdin);
      for(int i = 0; i < 63; i++)
        if (!line[i]) break;
        else if (line[i] == '\n') { line[i] = 0; break; }
      
      if (!strcmp(line, "") || !strcmp(line, "s")) {
        emu.debug.state.type = Debugger::State::STEP;
      }
      else if (!strcmp(line, "n")) {
        log("scanning to", emu._printer.PC);
        emu.debug.state.type = Debugger::State::RUN_TO;
        emu.debug.state.addr = emu._printer.PC;
        continue;
      }
      else if (!strcmp(line, "c")) {
        emu.debug.state.type = Debugger::State::RUN;
      }
      else if (!strcmp(line, "r")) {
        emu.debug.state.type = Debugger::State::RUN_TO_RET;
        emu.debug.state.call_depth = 1;
      }
      else if (!strcmp(line, "q")) {
        break;
      }
      else {
        continue;
      }
    }
      
    // 456 * 154 ticks is one emulator frame 
    // emu.step(456 * 154);
    emu.single_step();
  }
}

u32 display[144 * 160];
void _push_frame(u32 category, u8* memory, u32 len) {
  if (category == 0x300) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, win32_emulator.display_texture);
    glTexImage2D(
      GL_TEXTURE_2D, 0,
      GL_RED, // internal format
      160, 144,   //dimensions
      0 /*border*/,
      GL_RED /*format*/ ,
      GL_UNSIGNED_BYTE /*type*/,
      memory);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    #ifdef NOSWAP
    glFinish();
    #else
    SwapBuffers(win32_emulator.hdc);
    #endif
  }
}