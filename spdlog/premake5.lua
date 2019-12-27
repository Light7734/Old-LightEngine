project "spdlog"

    kind "StaticLib"
    staticruntime "on"

    cppdialect "C++17"
    language   "C++"

    targetdir (TargetDir)
    objdir    (ObjectDir)

    defines 
	{
		"_CRT_SECURE_NO_WARNINGS" ,
		"SPDLOG_COMPILED_LIB"     ,
	}

    files    "%{prj.location}/**.**"
    excludes "%{prj.location}/**.vcxproj**"
    
	includedirs
	{
		"%{prj.location}/"
	}

    -- Configurations
    filter "configurations:debug"
        defines  "LIGHT_DEBUG"
        optimize "debug"
        runtime  "debug"
        symbols  "on"

    filter "configurations:release"
        defines "LIGHT_RELEASE"
        optimize "on"
        runtime  "release"

    filter "configurations:distribution"
       defines "LIGHT_DIST"
       optimize "on"
       runtime  "release"