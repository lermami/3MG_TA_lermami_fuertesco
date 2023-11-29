-- premake5.lua

conan = {}
configs = {'Debug','Release','RelWithDebInfo'}

for i = 1,3 do
    include("build/deps/"..configs[i].."/conanbuildinfo.premake.lua")
      conan[configs[i]] = {}
      local cfg = conan[configs[i]]
      cfg["build_type"] = conan_build_type
      cfg["arch"] = conan_arch
      cfg["includedirs"] = conan_includedirs
      cfg["libdirs"] = conan_libdirs
      cfg["bindirs"] = conan_bindirs
      cfg["libs"] = conan_libs
      cfg["system_libs"] = conan_system_libs
      cfg["defines"] = conan_defines
      cfg["cxxflags"] = conan_cxxflags
      cfg["cflags"] = conan_cflags
      cfg["sharedlinkflags"] = conan_sharedlinkflags
      cfg["exelinkflags"] = conan_exelinkflags
      cfg["frameworks"] = conan_frameworks

end

function conan_config_exec()
  configs = {'Debug','Release','RelWithDebInfo'}
    for i = 1,3 do

    local cfg = conan[configs[i]]
    filter("configurations:"..configs[i])

      linkoptions { cfg["exelinkflags"] }
      includedirs{ cfg["includedirs"] }
      libdirs{ cfg["libdirs"] }
      links{ cfg["libs"] }
      links{ cfg["system_libs"] }
      links{ cfg["frameworks"] }
      defines{ cfg["defines"] }

     filter{}
  end
end

function conan_config_lib()
  configs = {'Debug','Release','RelWithDebInfo'}
  for i = 1,3 do
    local cfg = conan[configs[i]]
    filter("configurations:"..configs[i])
      linkoptions { cfg["sharedlinkflags"] }
      includedirs{ cfg["includedirs"] }
      defines{ cfg["defines"] }
    filter{}
  end
end

workspace "Motor"
  configurations { "Debug", "Release", "RelWithDebInfo" }
  architecture "x64"
  location "build"
  cppdialect "c++20"
  startproject "Window"

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
    runtime "Debug"

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
    runtime "Release"

  filter "configurations:RelWithDebInfo"
    defines { "NDEBUG" }
    optimize "On"
    runtime "Release"
    symbols "On"
  filter {}

project "Motor"
  kind "StaticLib"
  targetdir "build/%{cfg.buildcfg}"
  includedirs {
		"include", "include/math_library"
		}
  conan_config_lib()
  pchheader "stdafx.hpp"
  pchsource "src/stdafx.cpp"
  forceincludes { "stdafx.hpp" }

  files {
    "examples/window.cpp","src/*", "include/*", "include/math_library/*", "src/math_library/*"
  }

project"Window"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{prj.name}/%{cfg.buildcfg}"
  includedirs {
		"include", "include/math_library"
		}
  links "Motor"

  conan_config_exec("Debug")
  conan_config_exec("Release")
  conan_config_exec("RelWithDebInfo")
  debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }
  
  files {
    "examples/window.cpp"
  }

project"Triangle"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{prj.name}/%{cfg.buildcfg}"
  includedirs {
		"include", "include/math_library"
		}
  links "Motor"

  conan_config_exec("Debug")
  conan_config_exec("Release")
  conan_config_exec("RelWithDebInfo")
  debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }
  
  files {
    "examples/triangle.cpp"
  }

project"Input"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{prj.name}/%{cfg.buildcfg}"
  includedirs {
		"include", "include/math_library"
		}
  links "Motor"

  conan_config_exec("Debug")
  conan_config_exec("Release")
  conan_config_exec("RelWithDebInfo")
  debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }
  
  files {
    "examples/input.cpp"
  }

project"Triangle Many"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{prj.name}/%{cfg.buildcfg}"
  includedirs {
	"include", "include/math_library"
	}
  links "Motor"

  conan_config_exec("Debug")
  conan_config_exec("Release")
  conan_config_exec("RelWithDebInfo")
  debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }

files {
    "examples/triangle_many.cpp"

  }

project"Obj Many"
  kind "ConsoleApp"
  language "C++"
  targetdir "build/%{prj.name}/%{cfg.buildcfg}"
  includedirs {
	"include", "include/math_library"
	}
  links "Motor"

  conan_config_exec("Debug")
  conan_config_exec("Release")
  conan_config_exec("RelWithDebInfo")
  debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }

files {
    "examples/obj_many.cpp"

  }