#include "vulkan_api.hpp"
#include "vulkan_api_helpers.hpp"

#include <set>

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

void VulkanAPI::init_default()
{
    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "init default\n";

    create_vk_instance();
    setup_debug_cb();
    create_surface();
    selected_physical_device = choose_device_auto();
    create_logical_device( selected_physical_device );

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

    VkResult res = vkCreateDevice(dev, &create_info, nullptr, &device);

    if( res == VK_SUCCESS )
	std::cout << A_YELLOW << "[VAPI] " << A_RESET << "Logical Device Created\n";
    else
	std::cout << A_RED << "[VAPI] " << A_RESET << "Logical Device Failed to create\n";
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

	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Discrete GPU found\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Vulkan version supported: " << props.apiVersion << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << "Queue families check: " << queue_family_status << "\n";
	    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "     " << A_GREEN << "CHOSEN" << A_RESET << "\n";
	    return dev;
	}
    }

    return VK_NULL_HANDLE;
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
