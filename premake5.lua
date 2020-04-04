workspace "Light Engine"

    configurations 
    {
        "Debug"        ,
        "Release"      ,
        "Distribution" ,
    }

    architecture "x64"
    startproject "Demo"

TargetDir = "%{wks.location}bin/%{cfg.buildcfg}/%{prj.name}/"
ObjectDir = "%{wks.location}bin-obj/%{cfg.buildcfg}/%{prj.name}/"

DependenciesDir = "%{wks.location}/Dependencies/"

include "Light Engine/"
include "Sandbox/"
include "Demo/"
include "glfw/"
include "glad/"
include "ImGui/"
include "freetype/"
include "spdlog/"