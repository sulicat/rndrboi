#include "vulkan_api.hpp"
#include "vulkan_api_helpers.hpp"

#include <set>
#include <algorithm>
#include <limits>

using namespace rndrboi;

VulkanAPI* VulkanAPI::singleton_instance = NULL;

VulkanAPI* VulkanAPI::Instance()
{
    if (!singleton_instance)
    {
	singleton_instance = new VulkanAPI;
    }
    return singleton_instance;
}

//----------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, struct VulkanAPI::SwapChainSupportInfo& info)
{
    os << " CAP img_count { " << info.capabilities.currentExtent.width << "x" << info.capabilities.currentExtent.height << " }";
    os << " Formats { ";
    for( auto f : info.formats )
	os << f.format << " ";
    os << "}";

    os << " Color Space { ";
    for( auto f : info.formats )
	os << f.colorSpace << " ";
    os << "}";

    os << " PRESENT MODES { ";
    for( auto p : info.present_modes )
	os << p << " ";
    os << "}";

    return os;
}


void VulkanAPI::init_default()
{
    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "init default\n";

    create_vk_instance();
    setup_debug_cb();
    create_surface();
    selected_physical_device = choose_device_auto();
    create_logical_device( selected_physical_device );
    create_swapchain( selected_physical_device );
}

void VulkanAPI::update_physical_device_list()
{
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);
    devices = std::vector<VkPhysicalDevice>( device_count );
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());
}

void VulkanAPI::create_logical_device( VkPhysicalDevice dev )
{
    QueFamilyInfo selected_q_fam = que_family_info( dev );
    float q_prio = 1.0f;

    std::set<uint32_t> unique_que_index;
    for( auto i : selected_q_fam.graphics_family_indices )
    {
	unique_que_index.insert( i );
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Graphics queue index: " << i << "\n";
	break; // only 1 queue of every family type ;)
    }

    for( auto i : selected_q_fam.present_family_indices )
    {
	unique_que_index.insert( i );
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Present queue index: " << i << "\n";
	break; // only 1 queue of every family type ;)
    }

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    for( auto index : unique_que_index )
    {
	VkDeviceQueueCreateInfo queue_create_info{};
	queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_create_info.queueFamilyIndex = index;
	queue_create_info.queueCount = 1;
	queue_create_info.pQueuePriorities = &q_prio;
	queue_create_infos.push_back( queue_create_info );

	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Creating queue index: " << index << "\n";
    }

    VkPhysicalDeviceFeatures device_features{};

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = queue_create_infos.size();
    create_info.enabledExtensionCount = device_extensions.size();
    create_info.ppEnabledExtensionNames = device_extensions.data();

    VkResult res = vkCreateDevice(dev, &create_info, nullptr, &device);

    if( res == VK_SUCCESS )
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Logical Device Created\n";
    else
	std::cout << A_RED << "[VAPI] " << A_RESET << "Logical Device Failed to create\n";
}

void VulkanAPI::create_swapchain( VkPhysicalDevice dev  )
{
    SwapChainSupportInfo swap_chain_info = get_swapchain_support_info( dev );
    VkSurfaceFormatKHR chosen_surface_format = get_preferred_format( swap_chain_info.formats );
    VkPresentModeKHR chosen_present_mode = get_preferred_mode( swap_chain_info.present_modes );
    VkExtent2D chosen_extent = get_preferred_extent( swap_chain_info.capabilities );

    uint32_t image_count = swap_chain_info.capabilities.minImageCount + 1;

    VkSwapchainCreateInfoKHR create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_info.surface = surface;
    create_info.minImageCount = image_count;
    create_info.imageFormat = chosen_surface_format.format;
    create_info.imageColorSpace = chosen_surface_format.colorSpace;
    create_info.imageExtent = chosen_extent;
    create_info.imageArrayLayers = 1;
    create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueFamilyInfo q_fam_info = que_family_info( dev );

    if( q_fam_info.graphics_family_indices[0] == q_fam_info.present_family_indices[0] )
    {
	create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        create_info.queueFamilyIndexCount = 0;
	create_info.pQueueFamilyIndices = nullptr;
    }
    else
    {
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR Unhandled case of concurrent que fams\n";
    }

    create_info.preTransform = swap_chain_info.capabilities.currentTransform;
    create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_info.presentMode = chosen_present_mode;
    create_info.clipped = VK_FALSE;
    create_info.oldSwapchain = VK_NULL_HANDLE;

    VkResult stat = vkCreateSwapchainKHR( device,
					  &create_info,
					  nullptr,
					  &swap_chain );


    if( stat == VK_SUCCESS )
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Created Swapchain\n";
    else
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR failed to create swapchain\n";

    swapchain_images = get_swapchain_images( swap_chain );
    swapchain_image_extent = chosen_extent;
    swapchain_image_format = chosen_surface_format.format;
}

bool VulkanAPI::check_dev_extensions( VkPhysicalDevice dev )
{
    uint32_t extension_count;
    vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, nullptr);
    std::vector<VkExtensionProperties> physical_device_ext( extension_count );
    vkEnumerateDeviceExtensionProperties(dev, nullptr, &extension_count, physical_device_ext.data());


    for( auto wanted_ext : device_extensions )
    {
	bool found = false;
	for( auto ext : physical_device_ext )
	{
	    if( std::string(ext.extensionName) == wanted_ext )
	    {
		found = true;
		break;
	    }
	}

	if( found == false )
	{
	    std::cout << A_RED << "[VAPI] " << A_RESET << "Could not find dev ext: " << wanted_ext << " for " << dev << " \n";
	    return false;
	}
    }

    return true;
}

std::vector<VkImage> VulkanAPI::get_swapchain_images( VkSwapchainKHR sc )
{
    std::vector<VkImage> images;

    uint32_t image_count;
    vkGetSwapchainImagesKHR(device, sc, &image_count, nullptr);
    images.resize(image_count);
    vkGetSwapchainImagesKHR(device, sc, &image_count, images.data());

    return std::move(images);
}


VulkanAPI::SwapChainSupportInfo VulkanAPI::get_swapchain_support_info( VkPhysicalDevice dev )
{
    SwapChainSupportInfo info;

    // capabilities
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR( dev, surface, &info.capabilities);

    // format
    uint32_t format_count;
    vkGetPhysicalDeviceSurfaceFormatsKHR( dev, surface, &format_count, nullptr );
    if( format_count != 0 )
    {
	info.formats.resize( format_count );
	vkGetPhysicalDeviceSurfaceFormatsKHR( dev, surface, &format_count, info.formats.data() );
    }

    // present mode
    uint32_t present_mode_count;
    vkGetPhysicalDeviceSurfacePresentModesKHR( dev, surface, &present_mode_count, nullptr );
    if( present_mode_count != 0 )
    {
	info.present_modes.resize( present_mode_count );
	vkGetPhysicalDeviceSurfacePresentModesKHR( dev, surface, &present_mode_count, info.present_modes.data() );

    }



    return info;
}

void VulkanAPI::create_surface()
{
    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {

	rndrboi::WindowGLFW* glfw_window = dynamic_cast<rndrboi::WindowGLFW*>(Window::Instance()->get().get());

	VkResult res = glfwCreateWindowSurface(instance,
					       glfw_window->window,
					       nullptr,
					       &surface);

	if( res == VK_SUCCESS )
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Created GLFW Surface\n";
	else
	    std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR: Unable to create GLFW Surface\n";

    }
    else
    {
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR: Unable to create surface for window manager type\n";
    }
}

VkPhysicalDevice VulkanAPI::choose_device_auto()
{
    update_physical_device_list();

    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Choosing Device:\n";

    for( auto dev : devices )
    {
	VkPhysicalDeviceProperties props;
	VkPhysicalDeviceFeatures features;
	vkGetPhysicalDeviceProperties(dev, &props);
	vkGetPhysicalDeviceFeatures(dev, &features);

	std::cout << A_YELLOW << "[VAPI] " << A_RESET << " -> " << props.deviceName << "\n";

	// for now pick the first discrete GPU. We can optimize later
	if( props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
	{
	    bool queue_family_status = check_queue_families( dev );
	    bool device_extension_supp = check_dev_extensions( dev );
	    SwapChainSupportInfo swap_chain_info = get_swapchain_support_info( dev );

	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Discrete GPU found\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Vulkan version supported: " << props.apiVersion << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Queue families check: " << queue_family_status << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Extension support check: " << device_extension_supp << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "SwapChainInfo: " << swap_chain_info << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << A_GREEN << "CHOSEN" << A_RESET << "\n";
	    return dev;
	}
    }

    return VK_NULL_HANDLE;
}

VkSurfaceFormatKHR VulkanAPI::get_preferred_format( std::vector<VkSurfaceFormatKHR> format_in )
{
    for( auto f : format_in )
    {
	if( f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR )
	{
	    return f;
	}
    }
    return format_in[0];
}

VkPresentModeKHR VulkanAPI::get_preferred_mode( std::vector<VkPresentModeKHR> present_modes_in )
{
    for( auto pm : present_modes_in )
    {
	if( pm == VK_PRESENT_MODE_MAILBOX_KHR )
	{
	    std::cout << "IDEAL PRESENT MODE";
	    return pm;
	}
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanAPI::get_preferred_extent( VkSurfaceCapabilitiesKHR capabilities )
{

    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
	    return capabilities.currentExtent;
	}
	else
	{

	    rndrboi::WindowGLFW* glfw_window = dynamic_cast<rndrboi::WindowGLFW*>(Window::Instance()->get().get());

	    int width, height;
	    glfwGetFramebufferSize(glfw_window->window, &width, &height);

	    VkExtent2D actualExtent = {
		static_cast<uint32_t>(width),
		static_cast<uint32_t>(height)
	    };

	    actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
	    actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
	    return actualExtent;
	}
    }

    return capabilities.currentExtent;

}

VulkanAPI::QueFamilyInfo VulkanAPI::que_family_info( VkPhysicalDevice dev )
{
    QueFamilyInfo out;

    uint32_t q_fam_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &q_fam_count, nullptr);
    std::vector<VkQueueFamilyProperties> q_fams(q_fam_count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &q_fam_count, q_fams.data());

    for( int i = 0; i < q_fams.size(); i++ )
    {
	auto q = q_fams[i];

	// graphicss
	if ( q.queueFlags & VK_QUEUE_GRAPHICS_BIT)
	{
	    out.graphics_family_indices.push_back(i);
	}

	// presentation
	VkBool32 present_support = false;
	vkGetPhysicalDeviceSurfaceSupportKHR( dev, i, surface, &present_support );
	out.present_family_indices.push_back( i );

    }

    out.supports_graphics = out.graphics_family_indices.size() > 0;
    out.supports_present = out.present_family_indices.size() > 0;

    return out;
}


bool VulkanAPI::check_queue_families( VkPhysicalDevice dev )
{
    uint32_t q_fam_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &q_fam_count, nullptr);
    std::vector<VkQueueFamilyProperties> q_fams(q_fam_count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &q_fam_count, q_fams.data());

    bool found_graphics = false;

    for( int i = 0; i < q_fams.size(); i++ )
    {
	auto q = q_fams[i];

	if ( q.queueFlags & VK_QUEUE_GRAPHICS_BIT)
	    found_graphics = true;
    }

    return found_graphics;
}

void VulkanAPI::create_vk_instance()
{
    supported_extensions = get_supported_extentions();
    supported_layers = get_supported_layers();

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = "rndrboi app";
    app_info.applicationVersion = VK_MAKE_VERSION(0,0,1);
    app_info.pEngineName = "rndrboi";
    app_info.engineVersion = VK_MAKE_VERSION(0,0,1);


    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {
	uint32_t window_extension_count = 0;
	const char** window_extensions;
	window_extensions = glfwGetRequiredInstanceExtensions( &window_extension_count );

	for( int i = 0; i < window_extension_count; i++ )
	    extensions.push_back( window_extensions[i] );
    }

    for( const char* ext : added_extensions )
	extensions.push_back(ext);

    for( auto e : extensions )
	std::cout << A_YELLOW << "[VAPI] " << A_RESET
		  << "extension: " << e << "\n";


    bool ext_support_ok = check_extension_support();
    if( !ext_support_ok )
    {
	std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR: Insufficiant extension support\n";
	exit(-1);
    }


    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = 0;


    bool layer_support_ok = true;

    if( Config::Instance()->enable_debug )
    {
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "debug " << A_YELLOW << "ENABLED" << A_RESET << " ... using validation layers\n";

	layer_support_ok = check_layer_support();
	if( !layer_support_ok )
	{
	    std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR: Insufficiant validation layer support\n";
	    exit(-1);
	}

	// enable validation layers
	create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
	create_info.ppEnabledLayerNames = validation_layers.data();

    }
    else
    {
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "debug " << A_YELLOW << "DISABLED" << A_RESET << " ... no validation layers\n";
	// no validation layers
	create_info.enabledLayerCount = 0;
    }

    vkCreateInstance( &create_info,
		      nullptr,
		      &instance );

}

void VulkanAPI::cleanup()
{

    vkDestroySwapchainKHR(device, swap_chain, nullptr);
    vkDestroyDevice(device, nullptr);
    DestroyDebugUtilsMessengerEXT( instance, debug_messenger, nullptr );
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);

    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "cleanup\n";
}

std::vector<VkExtensionProperties> VulkanAPI::get_supported_extentions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> out( extensionCount );
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, out.data());
    return out;
}

std::vector<VkLayerProperties> VulkanAPI::get_supported_layers()
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> out( layer_count );
    vkEnumerateInstanceLayerProperties(&layer_count, out.data());
    return out;
}


bool VulkanAPI::check_layer_support()
{
    for( auto val_layer : validation_layers )
    {
	bool found = false;
	for( auto supp_layer : supported_layers )
	{
	    if( std::string(supp_layer.layerName) == std::string(val_layer) )
		found = true;
	}

	if( !found )
	{
	    std::cout << A_RED << "[VAPI] " << A_RESET << "cound not find: " << val_layer << "\n";
	    return false;
	}
    }

    return true;
}

bool VulkanAPI::check_extension_support()
{
    for( auto ext : extensions )
    {
	bool found = false;
	for( auto supp_ext : supported_extensions )
	{
	    if( std::string(supp_ext.extensionName) == ext )
		found = true;
	}

	if( !found )
	{
	    std::cout << A_RED << "[VAPI] " << A_RESET << "cound not find: " << ext << "\n";
	    return false;
	}

    }

    return true;
}

void VulkanAPI::setup_debug_cb()
{
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType		= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType	= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_cb;
    create_info.pUserData	= nullptr;


    CreateDebugUtilsMessengerEXT( instance,
				  &create_info,
				  nullptr,
				  &debug_messenger );

}
