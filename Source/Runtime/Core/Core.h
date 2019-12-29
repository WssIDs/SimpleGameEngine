#pragma once

// low level
#include <iterator>
#include <string>
#include <stdexcept>

// Types
#include <Runtime/Core/CoreTypes.h>
#include <Runtime/D3D11RHI/DX11/DX11RHI.h>


#include <Runtime/Logger/LogDefinitions.h>


// Helpers
#include <Runtime/Core/Helpers/DirectXHelper.h>
#include <Runtime/Core/Helpers/Path.h>
#include <Runtime/Core/Helpers/StringHelper.h>


// Math
#include <Runtime/Core/Math/Color.h>
#include <Runtime/Core/Math/WGMath.h>
#include <Runtime/Core/Math/XM.h>



// Object
#include <Runtime/Engine/WObject.h>
#include <Runtime/Engine/ObjectGlobals.h>


/// Imgui
#include <ThirdParty/Imgui/imgui.h>

#define BASE_SHADERS_DIR std::string("..\\..\\..\\Shaders\\")
#define BASE_MODELS_DIR std::string("..\\..\\..\\Content\\Models\\")
#define BASE_TEXTURES_DIR std::string("..\\..\\..\\Content\\Textures\\")