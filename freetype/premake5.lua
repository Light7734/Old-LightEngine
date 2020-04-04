project "freetype"

	kind "StaticLib"
	staticruntime "on"
	
	language   "C++"
	cppdialect "C++17"
	
	targetdir (TargetDir)
	objdir    (ObjectDir)

	warnings "off"

	defines 
	{
		"FT2_BUILD_LIBRARY"      ,
		"_CRT_SECURE_NO_WARNINGS",
	}

	files
	{
		"%{prj.location}/**.c"         ,
		"%{prj.location}/**.h"         ,
		"%{prj.location}/premake5.lua" ,
	}

	excludes 
	{
		"%{prj.location}/gzip/infblock.c",
		"%{prj.location}/gzip/infcodes.c",
		"%{prj.location}/gzip/inflate.c",
		"%{prj.location}/gzip/infutil.c",
	}

	includedirs
	{
		"%{prj.location}/",
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