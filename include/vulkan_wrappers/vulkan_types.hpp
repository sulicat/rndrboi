#pragma once

#include <vector>
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

#include "vulkan_wrappers/vulkan_vertex.hpp"


namespace rndrboi
{
    enum SWAPCHAIN_STATUS
    {
	OK,
	OUT_OF_DATE,
	SUBOPTIMAL
    };

    struct SwapChainSupportInfo
    {
	VkSurfaceCapabilitiesKHR	 capabilities;
	std::vector<VkSurfaceFormatKHR>	 formats;
	std::vector<VkPresentModeKHR>	 present_modes;
    };

    struct QueFamilyInfo
    {
	int	 graphics_family_index;
	int	 present_family_index;
	bool	 supports_graphics;
	bool	 supports_present;
    };

    struct VulkanDevice
    {
	VkInstance			 instance;
	VkPhysicalDevice		 physical_device	= VK_NULL_HANDLE;
	VkDevice			 logical_device		= VK_NULL_HANDLE;
	std::vector<const char*>         extensions;
	std::vector<const char*>         validation_layers;
	VkSurfaceKHR			 surface = NULL;
	VkDebugUtilsMessengerEXT	 debug_messenger;
	std::vector<VkPhysicalDevice>	 available_devices;
	SwapChainSupportInfo		 swapchain_info;
	QueFamilyInfo			 queue_fam_info;
	VkQueue				 graphics_queue;
	VkQueue				 present_queue;

    };

    struct VulkanDevicePreferences
    {
	bool graphics			= true;
	bool present			= true;
	bool dedicated_gpu		= true;
	bool limit_one_queue		= true;
	bool debug			= true;
	bool print_info			= true;
	bool geometry_shader		= false;
	bool tessellation_shader	= false;
	std::string app_name		= "rndrboiapp";
	std::string engine_name		= "rndrboi";
	int app_ver[3]			= {0,0,1};
	int engine_ver[3]		= {0,0,1};
    };


    enum BLEND_TYPE
    {
	OFF,
	ALPHA_BLEND
    };


    struct ShaderAttribute
    {
	uint32_t location;
	uint32_t offset;
	VkFormat format;
    };

    struct GraphicsPipelineSettings
    {
	std::string vert_shader_path;
	std::string frag_shader_path;
	std::string vert_entrypoint = "main";
	std::string frag_entrypoint = "main";
	float viewport_width = 1920;
	float viewport_height = 1080;

	VkPolygonMode polygon_mode	= VK_POLYGON_MODE_FILL;
	VkCullModeFlagBits cull_mode	= VK_CULL_MODE_BACK_BIT;
	VkFrontFace front_face		= VK_FRONT_FACE_CLOCKWISE;
	BLEND_TYPE blend_type		= OFF;

	std::vector<ShaderAttribute> shader_attributes =
	{ { 0, Vertex::offset_pos(), VK_FORMAT_R32G32B32_SFLOAT },
	  { 1, Vertex::offset_color(), VK_FORMAT_R32G32B32A32_SFLOAT } };

	// add constants for every stage
	// look at pSpecializationInfo here: https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPipelineShaderStageCreateInfo.html

	std::optional<std::string> tesselation_shader_path;
	std::optional<std::string> geometry_shader_path;
    };

    struct RenderPassSettings
    {
	VkFormat format;
    };

    struct CommandManagerSettings
    {
	int num_command_buffers = 1;
    };

    struct BufferSettings
    {
	uint32_t buffer_size = 1028;
	VkBufferUsageFlags usage = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    };

};
