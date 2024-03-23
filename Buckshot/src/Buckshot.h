#pragma once

// Header that includes all the functionalities that a Buckshot Application might use


// Core Components
#include "Buckshot/Core/Application.h"
#include "Buckshot/Core/MouseCodes.h"
#include "Buckshot/Core/Timestep.h"
#include "Buckshot/Core/KeyCodes.h"
#include "Buckshot/Core/Input.h"
#include "Buckshot/Core/Layer.h"
#include "Buckshot/Core/Base.h"
#include "Buckshot/Core/Log.h"

// GUI
#include "Buckshot/ImGui/ImGuiLayer.h"

// Rendering
#include "Buckshot/Renderer/OrthographicCameraController.h"
#include "Buckshot/Renderer/OrthographicCamera.h"
#include "Buckshot/Renderer/RenderCommand.h"
#include "Buckshot/Renderer/SubTexture2D.h"
#include "Buckshot/Renderer/Framebuffer.h"
#include "Buckshot/Renderer/VertexArray.h"
#include "Buckshot/Renderer/Renderer2D.h"
#include "Buckshot/Renderer/Renderer.h"
#include "Buckshot/Renderer/Buffers.h"
#include "Buckshot/Renderer/Texture.h"
#include "Buckshot/Renderer/Shader.h"

// Scene Management
#include "Buckshot/Scene/SceneSerializer.h"
#include "Buckshot/Scene/ScriptableEntity.h"
#include "Buckshot/Scene/Components.h"
#include "Buckshot/Scene/Entity.h"
#include "Buckshot/Scene/Scene.h"