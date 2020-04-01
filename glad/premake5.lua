project "glad"

	kind "StaticLib"
	staticruntime "on"
	
	language   "C++"
	cppdialect "C++17"
	
	targetdir (TargetDir)
	objdir    (ObjectDir)

	warnings "off"

	files
	{
		"%{prj.location}/**.c"         ,
		"%{prj.location}/**.h"         ,
		"%{prj.location}/premake5.lua" ,
	}

	includedirs
	{
		"%{prj.location}/"
	}
	
	filter "configurations:debug"
		optimize "on"
		runtime "debug"
		
	filter "configurations:release"
		optimize "on"
		runtime "release"
		
	filter "configurations:distrubution"
		optimize "on"
		runtime "release"
