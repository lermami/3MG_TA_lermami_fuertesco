#pragma once

#include <vector>

enum Key {
  kKey_Aphostrophe = 39,
  kKey_Coma = 44,
  kKey_Hyphen,//
  kKey_Dot,
  kKey_0 = 48,
  kKey_1,
  kKey_2,
  kKey_3,
  kKey_4,
  kKey_5,
  kKey_6,
  kKey_7,
  kKey_8,
  kKey_9,
  kKey_Semicolon  = 59,
  kKey_A = 65,
  kKey_B,
  kKey_C,
  kKey_D,
  kKey_E,
  kKey_F,
  kKey_G,
  kKey_H,
  kKey_I,
  kKey_J,
  kKey_K,
  kKey_L,
  kKey_M,
  kKey_N,
  kKey_O,
  kKey_P,
  kKey_Q,
  kKey_R,
  kKey_S,
  kKey_T,
  kKey_U,
  kKey_V,
  kKey_W,
  kKey_X,
  kKey_Y,
  kKey_Z,
  kKey_Quote = 96,//
  kKey_Delete = 127,//
  kKey_Degree = 248,//
  kKey_Diaeresis,  //
  /*
kKey_KEY_UNKNOWN   -1
kKey_KEY_SPACE   32
kKey_KEY_APOSTROPHE   39 
kKey_KEY_COMMA   44 
kKey_KEY_MINUS   45 /
kKey_KEY_PERIOD   46 /
kKey_KEY_SLASH   47 /
kKey_KEY_0   48
kKey_KEY_1   49
kKey_KEY_2   50
kKey_KEY_3   51
kKey_KEY_4   52
kKey_KEY_5   53
kKey_KEY_6   54
kKey_KEY_7   55
kKey_KEY_8   56
kKey_KEY_9   57
kKey_KEY_SEMICOLON   59 /
kKey_KEY_EQUAL   61 /
kKey_KEY_A   65
kKey_KEY_B   66
kKey_KEY_C   67
kKey_KEY_D   68
kKey_KEY_E   69
kKey_KEY_F   70
kKey_KEY_G   71
kKey_KEY_H   72
kKey_KEY_I   73
kKey_KEY_J   74
kKey_KEY_K   75
kKey_KEY_L   76
kKey_KEY_M   77
kKey_KEY_N   78
kKey_KEY_O   79
kKey_KEY_P   80
kKey_KEY_Q   81
kKey_KEY_R   82
kKey_KEY_S   83
kKey_KEY_T   84
kKey_KEY_U   85
kKey_KEY_V   86
kKey_KEY_W   87
kKey_KEY_X   88
kKey_KEY_Y   89
kKey_KEY_Z   90
kKey_KEY_LEFT_BRACKET   91 
kKey_KEY_BACKSLASH   92 /
kKey_KEY_RIGHT_BRACKET   93 /* ]
kKey_KEY_GRAVE_ACCENT   96 /* ` 
kKey_KEY_WORLD_1   161 /* non-US #1 
kKey_KEY_WORLD_2   162 /* non-US #2 
kKey_KEY_ESCAPE   256
kKey_KEY_ENTER   257
kKey_KEY_TAB   258
kKey_KEY_BACKSPACE   259
kKey_KEY_INSERT   260
kKey_KEY_DELETE   261
kKey_KEY_RIGHT   262
kKey_KEY_LEFT   263
kKey_KEY_DOWN   264
kKey_KEY_UP   265
kKey_KEY_PAGE_UP   266
kKey_KEY_PAGE_DOWN   267
kKey_KEY_HOME   268
kKey_KEY_END   269
kKey_KEY_CAPS_LOCK   280
kKey_KEY_SCROLL_LOCK   281
kKey_KEY_NUM_LOCK   282

kKey_KEY_PRINT_SCREEN   283

kKey_KEY_PAUSE   284

kKey_KEY_F1   290

kKey_KEY_F2   291

kKey_KEY_F3   292

kKey_KEY_F4   293

kKey_KEY_F5   294

kKey_KEY_F6   295

kKey_KEY_F7   296

kKey_KEY_F8   297

kKey_KEY_F9   298

kKey_KEY_F10   299

kKey_KEY_F11   300

kKey_KEY_F12   301

kKey_KEY_F13   302

kKey_KEY_F14   303

kKey_KEY_F15   304

kKey_KEY_F16   305

kKey_KEY_F17   306

kKey_KEY_F18   307

kKey_KEY_F19   308

kKey_KEY_F20   309

kKey_KEY_F21   310

kKey_KEY_F22   311

kKey_KEY_F23   312

kKey_KEY_F24   313

kKey_KEY_F25   314

kKey_KEY_KP_0   320

kKey_KEY_KP_1   321

kKey_KEY_KP_2   322

kKey_KEY_KP_3   323

kKey_KEY_KP_4   324

kKey_KEY_KP_5   325

kKey_KEY_KP_6   326

kKey_KEY_KP_7   327

kKey_KEY_KP_8   328

kKey_KEY_KP_9   329

kKey_KEY_KP_DECIMAL   330

kKey_KEY_KP_DIVIDE   331

kKey_KEY_KP_MULTIPLY   332

kKey_KEY_KP_SUBTRACT   333

kKey_KEY_KP_ADD   334

kKey_KEY_KP_ENTER   335

kKey_KEY_KP_EQUAL   336

kKey_KEY_LEFT_SHIFT   340

kKey_KEY_LEFT_CONTROL   341

kKey_KEY_LEFT_ALT   342

kKey_KEY_LEFT_SUPER   343

kKey_KEY_RIGHT_SHIFT   344

kKey_KEY_RIGHT_CONTROL   345

kKey_KEY_RIGHT_ALT   346

kKey_KEY_RIGHT_SUPER   347

kKey_KEY_MENU   348

kKey_KEY_LAST   _KEY_MENU
*/
};

class Window;

enum InputState {
  kInactive,
  kDown,
  kPressed,
  kUp,
};

class Input {
public:

  Input(int key);
  ~Input() = default;
  void update();

  bool IsKeyDown();
  bool IsKeyPressed();
  bool IsKeyUp();

  void setState(unsigned int state);
  unsigned int getState() const;

  int getKey() const;
private:
  /*
  0 -> Inactive
  1 -> Key down
  2 -> Key pressed
  3 -> Key up
  */
  unsigned int state_;
  int key_;
};

class InputMap {
public:
  InputMap(Window& w);
  ~InputMap();

  void addInput(Input* new_input);
  void updateInputs();

private:
  std::vector<Input*> inputmap_;
  window* windowHandle_;
};
