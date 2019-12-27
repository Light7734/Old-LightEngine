project "Light Engine"

    kind "StaticLib"
    staticruntime "on"

    cppdialect "C++17"
    language   "C++"

    targetdir (TargetDir)
    objdir    (ObjectDir)

    defines "_CRT_SECURE_NO_WARNINGS"

    files "%{prj.location}/**.**"
    excludes "%{prj.location}/**.vcxproj**"

	pchheader "ltpch.h"
	pchsource "src/Engine/ltpch.cpp"

    links
    {
        "spdlog"       ,
		"glfw"         ,
		"glad"         ,
        "opengl32.lib" ,
    }

    includedirs
    {
		"%{prj.location}/src/Engine/"  ,
		"%{prj.location}/src/"         ,
		"%{wks.location}/spdlog/"      ,
		"%{wks.location}/glfw/include" ,
		"%{wks.location}/glad/"        ,
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