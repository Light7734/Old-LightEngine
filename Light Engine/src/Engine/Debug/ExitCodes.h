#pragma once

/*
*
* -1 unsupported
*  0 successful
*  
* 1x assertions
* 2x not initialized
* 3x multiple instances
* 4x (function) call failed
* 5x invalid values
* 6x file read failed
* 7x cast failed
*
*/

#define EC_SUCCESSFUL_EXECUTION     0

#define EC_FEATURE_NOT_SUPPORTED    -1

#define EC_ENGINE_ASSERTION    10 // used for unspecified engine errors' code
#define EC_CLIENT_ASSERTION    11
#define EC_IMGUI_ASSERTION     12

#define EC_NO_INIT_APPLICATION          20
#define EC_NO_INIT_WINDOW               21
#define EC_NO_INIT_GRAPHICSC_CONTEXT    22

#define EC_MULTIPLE_INSTANCES_APPLICATION    30
#define EC_MULTIPLE_INSTANCES_WINDOW         31

#define EC_CALL_FAIL_GLFW_INIT             40
#define EC_CALL_FAIL_GLAD_LOAD             41
#define EC_CALL_FAIL_GLFW_CREATE_WINDOW    42

#define EC_INVALID_GRAPHICS_API    50
#define EC_INVALID_VERTEX_TYPE     51

#define EC_FILE_READ_FAIL_SHADER    60

#define EC_CAST_FAIL    70