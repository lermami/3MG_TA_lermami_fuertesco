#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Engine.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "soundmanager.h"
#include "soundsource.h"
#include "soundbuffer.h"

int main(int, char**) {
  Engine e;

  auto maybe_w = Window::create(e, 1024, 768, "Test Window");
  if (!maybe_w) return -1;

  auto& w = maybe_w.value();
  w.clearColor(0.4f, 0.4f, 0.4f, 1.0f);

  if (glewInit() != GLEW_OK) return -1;

  w.initImGui();

  SoundManager soundM_;

  ALCdevice* device = alcOpenDevice(NULL);
  ALCcontext* ctx = alcCreateContext(device, NULL);
  alcMakeContextCurrent(ctx);

  ALfloat a[] = { 0, 0, 0 };
  ALfloat b[] = { 0, 0, 0 };
  SoundSource src1 = SoundSource("Juan Pis", a, b, 0);

  SoundBuffer buf1 =
    SoundBuffer::MakeBuffer("assets/test.wav").value();

  src1.addSound(&buf1);

  soundM_.addsrc(src1);
  soundM_.addBuffer(buf1);

  while (!w.is_done()) {
    w.calculateLastTime();
    glClear(GL_COLOR_BUFFER_BIT);
    w.updateImGui();

    soundM_.SoundsInfoImgui();

    w.swap();

    w.calculateCurrentTime();
  }

  return 0;
}