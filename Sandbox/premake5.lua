project "Sandbox"

    kind "ConsoleApp"
    staticruntime "on"

    cppdialect "C++17"
    language   "C++"

    targetdir (TargetDir)
    objdir    (ObjectDir)

    defines "_CRT_SECURE_NO_WARNINGS"

    files    "%{prj.location}/**.**"
    excludes "%{prj.location}/**.vcxproj**"


    links
    {
		"Light Engine" ,
		"spdlog"       ,
		"opengl32.lib" ,
    }

    includedirs
    {
		"%{wks.location}/Light Engine/src/Engine/" ,
		"%{wks.location}/Light Engine/src"         ,
		"%{wks.location}/glfw/include"             ,
		"%{wks.location}/glad/"                    ,
		"%{wks.location}/ImGui/"                   ,
		"%{wks.location}/spdlog/"                  ,
		"%{wks.location}/Dependencies/glm/"        ,
		"%{wks.location}/Dependencies/irrKlang/include" ,
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
		postbuildcommands
		{
			"copy \"%{wks.location}/Dependencies/irrKlang/lib/ikpFlac.dll/ "  .. TargetDir .. "\"" ,
			"copy \"%{wks.location}/Dependencies/irrKlang/lib/ikpMP3.dll/ "   .. TargetDir .. "\"" ,
			"copy \"%{wks.location}/Dependencies/irrKlang/lib/irrKlang.dll/ " .. TargetDir .. "\"" ,
			"copy \"%{prj.location}/res " .. TargetDir .. "\"" 
		}
		defines "LIGHT_DIST"
		optimize "on"
		runtime  "release"