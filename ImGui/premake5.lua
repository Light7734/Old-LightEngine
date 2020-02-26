project "ImGui"

	kind "StaticLib"
	staticruntime "on"

	language   "C++"
	cppdialect "C++17"
	
	targetdir (TargetDir)
	objdir    (ObjectDir)

	defines "_CRT_SECURE_NO_WARNINGS"
	
    files "%{prj.location}/**.**"
    excludes "%{prj.location}/**.vcxproj**"

	includedirs
	{
		"%{wks.location}/glad/" ,
		"%{wks.location}/glfw/include" ,
	}

	filter "system:not windows"
		excludes
		{
			"imgui_impl_dx11.h"   ,
			"imgui_impl_dx11.cpp" ,
			"imgui_impl_win32.h"  ,
			"imgui_impl_win32.cpp",
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