#pragma once

/**
 * @brief Options for specifying the front face culling direction.
 */
enum class FrontFace {
  /**< Clock-wise winding order is considered the front face. */
  kClockWise,
  /**< Counter-clockwise winding order is considered the front face. */
  kCounterClockWise
};

/**
 * @brief Options for specifying which faces to cull in the rendering pipeline.
 */
enum class CullingMode {
  /**< Cull only the back face (counter-clockwise winding order). */
  kFrontLeft,
  /**< Cull only the front face (clockwise winding order). */
  kFrontRight,
  /**< Cull only the back face (clockwise winding order). */
  kBackLeft,
  /**< Cull only the front face (counter-clockwise winding order). */
  kBackRight,
  /**< Cull only the front face. */
  kFront,
  /**< Cull only the back face. */
  kBack,
  /**< Cull only the left face. */
  kLeft,
  /**< Cull only the right face. */
  kRight,
  /**< Cull both the front and back faces. */
  kFrontAndBack
};

/**
 * @brief Options for specifying depth testing behavior in the fragment shader.
 */
enum class DepthTestMode {
  /**< Never pass the depth test. */
  kNever,
  /**< Pass the depth test if the fragment's depth is less than the current depth buffer value. */
  kLess,
  /**< Pass the depth test if the fragment's depth is equal to the current depth buffer value. */
  kEqual,
  /**< Pass the depth test if the fragment's depth is greater than the current depth buffer value. */
  kGreater
};

/**
 * @brief Options for specifying the projection mode for the camera.
 */
enum class ProjectionMode {
  /**< Perspective projection, commonly used for 3D scenes. */
  kPerspective,
  /**< Orthographic projection, commonly used for 2D or UI elements. */
  kOrthogonal
};

/**
 * @brief Options for specifying the target of a texture (1D, 2D, or 3D).
 */
enum class TextureTarget {
  /**< Texture with one dimension (typically used for grayscale data). */
  kTexture_1D,
  /**< Texture with two dimensions (most common texture type). */
  kTexture_2D,
  /**< Texture with three dimensions (used for volumetric data). */
  kTexture_3D
};

/**
 * @brief Options for specifying the internal format of a texture (data type).
 */
enum class TextureType {
  /**< Unsigned byte (8-bit) data type for textures. */
  kUnsignedByte,
  /**< Float data type (32-bit) for textures. */
  kFloat
};

/**
 * @brief Options for specifying the texture format (number of components and color space).
 */
enum class TextureFormat {
  /**< Red, Green, and Blue color channels. */
  kRGB,
  /**< Red, Green, Blue, and Alpha channels. */
  kRGBA,
  /**< Depth component only (used for depth maps). */
  kDepthComponent
};

/**
 * @brief Options for specifying texture filtering behavior for minification and magnification.
 */
enum class TextureFiltering {
  /**< Nearest neighbor filtering (chooses the nearest texel). */
  kNearest,
  /**< Linear filtering (interpolates between neighboring texels). */
  kLinear
};

/**
 * @brief Options for specifying texture wrapping behavior at the borders.
 */
enum class TextureWrap {
  /**< Repeats the texture beyond its boundaries. */
  kRepeat,
  /**< Mirrors the texture at the borders (creating a repeating mirrored effect). */
  kMirroredRepeat,
  /**< Clamps the texture coordinates to the edge of the texture (no wrapping). */
  kClampToEdge,
  /**< Clamps the texture coordinates to the edge and uses a user-defined border color. */
  kClampToBorder
};

/**
 * @brief Types of lights supported by the engine.
 */
enum class LightType {
  kAmbient = 0,
  kDirectional = 1,
  kPoint = 2,
  kSpot = 3,
};

/**
 * @brief Enumeration of all supported keyboard keys.
 */
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

/**
 * @brief Enumeration of possible input states for a key or mouse button.
 */
enum class InputState {
  kInactive,  // Key or button is not pressed or released
  kDown,      // Key or button is currently pressed
  kPressed,   // Key or button was just pressed this frame
  kUp,        // Key or button was just released this frame
};

/**
 * @brief Enumeration of OpenGL buffer targets used for data binding.
 */
enum class Target {
  kTarget_VertexData,  // Target for vertex data (e.g., VBO)
  kTarget_Elements    // Target for element data (e.g., EBO)
};

/**
 * @brief Enumeration of predefined shader types offered by the engine.
 */
enum class ShaderType {
  kCustom,             // Custom shader program from user-defined source code
  kTexture,             // Shader for rendering with textures
  kTextureLight,        // Shader for rendering with textures and lighting
  kTextureLightShadow,  // Shader for rendering with textures, lighting, and shadows
  kColor,               // Shader for rendering with basic color information
  kColorLight,          // Shader for rendering with basic color and lighting
  kColorLightShadow     // Shader for rendering with basic color, lighting, and shadows
};