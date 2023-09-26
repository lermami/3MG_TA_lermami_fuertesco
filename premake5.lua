-- premake5.lua

include("conanbuildinfo.premake.lua")

workspace "HelloWorld"
   configurations { "Debug", "Release", "RelWithDebInfo" }
   location "build"
   conan_basic_setup()

project "HelloWorld"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   location "build"

   linkoptions { conan_exelinkflags }

   files { "**.h", "**.cc" }

   filter "configurations:Debug"
      defines { "Debug" }
      symbols "On"

   filter "configurations:Release"
      defines { "Release" }
      optimize "On"

    filter "configurations:RelWithDebInfo"
      defines { "RelWithDebInf" }
      optimize "On"
      symbols "On"
