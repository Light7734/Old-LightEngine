project "glfw"

	kind "StaticLib"
	staticruntime "on"

	language   "C++"
	cppdialect "C++17"
	
	targetdir (TargetDir)
	objdir    (ObjectDir)


	
	defines "_GLFW_WIN32"
	defines "_CRT_SECURE_NO_WARNINGS"

	files
	{
		"%{prj.location}/**.c"         ,
		"%{prj.location}/**.cpp"       ,
		"%{prj.location}/**.h"         ,
		"%{prj.location}/premake5.lua" ,
	}
	
	filter "platforms:windows"

	filter "platforms:linux"

	filter "platforms:mac"
	

	filter "configurations:debug"
		optimize "debug"
		runtime "debug"
		
	filter "configurations:release"
		optimize "on"
		runtime "release"
		
	filter "configurations:distrubution"
		optimize "on"
		runtime "release"