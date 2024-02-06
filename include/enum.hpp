#pragma once

//Render options
enum class FrontFace {
  kClockWise = 0x0900,
  kCounterClockWise = 0x0901
};

enum class CullingMode {
  kFrontLeft = 0x0400,
  kFrontRight = 0x0401,
  kBackLeft = 0x0402,
  kBackRight = 0x0403,
  kFront = 0x0404,
  kBack = 0x0405,
  kLeft = 0x0406,
  kRight = 0x0407,
  kFrontAndBack = 0x0408,
};

enum class DepthTestMode {
  kNever = 0x0200,
  kLess = 0x0201,
  kEqual = 0x0202,
  kGreater = 0x0204,
};

enum class ProjectionMode {
  kPerspective,
  kOrthogonal,
};

//Texture options
enum class TextureType {
  kTexture_1D = 0x0DE0,
  kTexture_2D = 0x0DE1,
  kTexture_3D = 0x806F,
};

enum class TextureFormat {
  kRGB = 0x1907,
  kRGBA = 0x1908,
};

enum class TextureParameters {
  kRepeat,
  kLinear,
};

//Light
enum class LightType {
  kAmbient = 0,
  kDirectional = 1,
  kPoint = 2,
  kSpot = 3,
};

//Input
enum KeyEnum {
  kKey_LeftClick = 0,
  kKey_RightClick,
  kKey_MiddleClick,
  kKey_AditionalMouseButton1,
  kKey_AditionalMouseButton2,
  kKey_AditionalMouseButton3,
  kKey_AditionalMouseButton4,
  kKey_Space = 32,
  kKey_Aphostrophe = 39,
  kKey_Coma = 44,
  kKey_Dot,
  kKey_Slash = 47,
  kKey_0,
  kKey_1,
  kKey_2,
  kKey_3,
  kKey_4,
  kKey_5,
  kKey_6,
  kKey_7,
  kKey_8,
  kKey_9,
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
  kKey_LeftBracket = 91,
  kKey_RightBracket = 93,
  kKey_Escape = 256,
  kKey_Enter,
  kKey_Tab,
  kKey_BackSpace,
  kKey_Insert,
  kKey_Delete,
  kKey_Right,
  kKey_Left,
  kKey_Down,
  kKey_Up,
  kKey_PageUp,
  kKey_PageDown,
  kKey_Start,
  kKey_End,
  kKey_CapsLock = 280,
  kKey_ScrollLock,
  kKey_NumLock,
  kKey_Pause = 284,
  kKey_F1 = 290,
  kKey_F2,
  kKey_F3,
  kKey_F4,
  kKey_F5,
  kKey_F6,
  kKey_F7,
  kKey_F8,
  kKey_F9,
  kKey_F10,
  kKey_F11,
  kKey_F12,
  kKey_LeftShift = 340,
  kKey_LeftControl,
  kKey_LeftAlt,
  kKey_LeftSuper,
  kKey_RightShift,
  kKey_RightControl,
  kKey_RightAlt,
  kKey_RightSuper,
  kKey_Menu,
};

enum InputState {
  kInactive,
  kDown,
  kPressed,
  kUp,
};

//OpenGL API
enum Target {
  kTarget_VertexData,
  kTarget_Elements,
};