project "glfw"

	kind "StaticLib"
	staticruntime "on"

	language   "C++"
	cppdialect "C++17"
	
	targetdir (TargetDir)
	objdir    (ObjectDir)

	defines "_CRT_SECURE_NO_WARNINGS"
	
	filter "system:windows"
		defines "_GLFW_WIN32"
		files
		{
			"GLFWSource/win32**"       ,
			"GLFWSource/context.c"     ,
			"GLFWSource/egl**"         ,
			"GLFWSource/init.c"        ,
			"GLFWSource/input.c"       ,
			"GLFWSource/internal.h"    ,
			"GLFWSource/mappings.h"    ,
			"GLFWSource/monitor.c"     ,
			"GLFWSource/nsgl_context.h",
			"GLFWSource/osmesa**"      ,
			"GLFWSource/wgl**"         ,
			"GLFWSource/window.c"      ,
			"GLFWSource/xkb**"         ,
			"GLFWSource/premake5.lua"  ,
		}

	filter "system:linux"
	-- #todo
	filter "system:mac"
	-- #todo

	filter "configurations:debug"
		optimize "debug"
		runtime "debug"
		
	filter "configurations:release"
		optimize "on"
		runtime "release"
		
	filter "configurations:distrubution"
		optimize "on"
		runtime "release"