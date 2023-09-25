-- premake5.lua
workspace "HelloWorld"
   configurations { "Debug", "Release", "RelWithDebInfo" }
   location "build"

project "HelloWorld"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   location "build"

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
