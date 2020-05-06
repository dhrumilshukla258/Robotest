/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Author: Dhrumil Shukla, dhrumil.shukla 60001718
Author: Fenil Shingala, fenil.shingala, 60003118
- End Header --------------------------------------------------------*/
#pragma once

// I/O
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string>

// STL
#include <unordered_map>
#include <forward_list>
#include <map>
#include <vector>
#include <stack>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <rapidjson/document.h>
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

// IMGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

// Logger
#include "Utilities/Logger.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// RTTR
#include <rttr/array_range.h>
#include <rttr/constructor.h>
#include <rttr/destructor.h>
#include <rttr/enumeration.h>
#include <rttr/method.h>
#include <rttr/property.h>
#include <rttr/registration.h>
#include <rttr/rttr_cast.h>
#include <rttr/rttr_enable.h>
#include <rttr/type.h>

//SCRIPTING
#include "../Resources/Includes/LUA/lua.hpp"
#include "../Resources/Includes/SOL/sol.hpp"