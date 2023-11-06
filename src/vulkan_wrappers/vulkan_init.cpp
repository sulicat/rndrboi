#include "vulkan_wrappers/vulkan_init.hpp"
#include "vulkan_wrappers/vulkan_api_helpers.hpp"
#include <set>

#define OK_PRINT (A_YELLOW "[VULKAN INIT] " A_RESET)
#define BAD_PRINT (A_RED "[VULKAN INIT] " A_RESET)

using namespace rndrboi;

static std::vector<const char*> validation_layers = {
    "VK_LAYER_KHRONOS_validation"
};

static std::vector<const char*> added_extensions = {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME
};

static std::vector<const char*> device_extensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void VulkanDeviceInit::setup_debug_cb( VulkanDevice& dev, const VulkanDevicePreferences& pref )
{
    VkDebugUtilsMessengerCreateInfoEXT create_info{};
    create_info.sType       = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    create_info.pfnUserCallback = debug_cb;
    create_info.pUserData   = nullptr;

    CreateDebugUtilsMessengerEXT( dev.instance,
                  &create_info,
                  nullptr,
                  &dev.debug_messenger );
}

std::vector<VkExtensionProperties> VulkanDeviceInit::get_supported_extentions()
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> out( extensionCount );
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, out.data());
    return out;
}

std::vector<VkLayerProperties> VulkanDeviceInit::get_supported_layers()
{
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    std::vector<VkLayerProperties> out( layer_count );
    vkEnumerateInstanceLayerProperties(&layer_count, out.data());
    return out;
}

VulkanDevice VulkanDeviceInit::init( const VulkanDevicePreferences& pref )
{
    std::cout << OK_PRINT << "Initializing\n";

    VulkanDevice out;
    create_instance( out, pref );
    setup_debug_cb( out, pref );
    create_surface( out, pref );
    choose_physical_device( out, pref );
    create_logical_device( out, pref );

    return std::move(out);
}

void VulkanDeviceInit::create_surface( VulkanDevice& dev, const VulkanDevicePreferences& pref )
{
    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {

    rndrboi::WindowGLFW* glfw_window = dynamic_cast<rndrboi::WindowGLFW*>(Window::Instance()->get().get());

    VkResult res = glfwCreateWindowSurface(dev.instance,
                           glfw_window->window,
                           nullptr,
                           &dev.surface);

    if( res == VK_SUCCESS )
        std::cout << OK_PRINT << "Created GLFW Surface\n";
    else
        std::cout << BAD_PRINT << "ERROR: Unable to create GLFW Surface\n";

    }
    else
    {
    std::cout << A_RED << "[VAPI] " << A_RESET << "ERROR: Unable to create surface for window manager type\n";
    }
}

void VulkanDeviceInit::choose_physical_device( VulkanDevice& dev_out, const VulkanDevicePreferences& pref )
{
    // get list of devices
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(dev_out.instance, &device_count, nullptr);
    dev_out.available_devices.resize(device_count);
    vkEnumeratePhysicalDevices(dev_out.instance, &device_count, dev_out.available_devices.data());

    std::cout << OK_PRINT << "Choosing Device\n";

    for( auto dev : dev_out.available_devices )
    {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceProperties(dev, &props);
    vkGetPhysicalDeviceFeatures(dev, &features);

    std::cout << OK_PRINT << " -> " << props.deviceName << "\n";

    // for now pick the first discrete GPU. We can optimize later
    if( props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU )
    {
        // perform more checks in the future
        std::cout << OK_PRINT << "     " << "Discrete GPU found\n";
        std::cout << OK_PRINT << "     " << "Vulkan version supported: " << props.apiVersion << "\n";
        std::cout << OK_PRINT << "     " << A_GREEN << "CHOSEN" << A_RESET << "\n";

        SwapChainSupportInfo swap_chain_info = get_swapchain_support_info( dev, dev_out.surface );
        dev_out.physical_device = dev;
        dev_out.max_anisotropy = props.limits.maxSamplerAnisotropy;
        dev_out.swapchain_info = swap_chain_info;
        return;
    }
    }
}


void VulkanDeviceInit::update_swapchain_info( VulkanDevice& dev )
{
    SwapChainSupportInfo swap_chain_info = get_swapchain_support_info( dev.physical_device, dev.surface );
    dev.swapchain_info = swap_chain_info;
}

SwapChainSupportInfo VulkanDeviceInit::get_swapchain_support_info( VkPhysicalDevice& dev, VkSurfaceKHR& surface )
{
    if( surface == NULL )
    std::cout << BAD_PRINT << "Surface not intialized, cannot get swapchain info\n";

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


bool VulkanDeviceInit::create_instance( VulkanDevice& dev, const VulkanDevicePreferences& pref )
{
    std::cout << OK_PRINT << "Creating instance\n";

    VkApplicationInfo app_info{};
    app_info.sType      = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = pref.app_name.c_str();
    app_info.applicationVersion = VK_MAKE_VERSION( pref.app_ver[0], pref.app_ver[1], pref.app_ver[2] );
    app_info.pEngineName    = pref.engine_name.c_str();
    app_info.engineVersion  = VK_MAKE_VERSION( pref.engine_ver[0], pref.engine_ver[1], pref.engine_ver[2] );

    // add the GLFW extention optionnaly
    if( Config::Instance()->window_manager == Config::WINDOW_MANAGER::GLFW )
    {
    uint32_t window_extension_count = 0;
    const char** window_extensions;
    window_extensions = glfwGetRequiredInstanceExtensions( &window_extension_count );

    for( int i = 0; i < window_extension_count; i++ )
        dev.extensions.push_back( window_extensions[i] );
    }

    // add any desired extensions
    for( const char* ext : added_extensions )
    dev.extensions.push_back(ext);

    for( auto e : dev.extensions )
    std::cout << OK_PRINT << "extension: " << e << "\n";


    dev.validation_layers = validation_layers;

    bool layers_supported = check_layer_support( dev.validation_layers );
    bool extensions_supported = check_extension_support( dev.extensions );
    std::cout << OK_PRINT << "Validation Layers Supported: " << layers_supported << "\n";
    std::cout << OK_PRINT << "Extensions Supported: " << extensions_supported << "\n";

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = dev.extensions.size();
    create_info.ppEnabledExtensionNames = dev.extensions.data();
    create_info.enabledLayerCount = 0;

    if( pref.debug )
    {
    std::cout << OK_PRINT << "DEBUG Enabled: using validation layers\n";
    create_info.enabledLayerCount = static_cast<uint32_t>(dev.validation_layers.size());
    create_info.ppEnabledLayerNames = dev.validation_layers.data();
    }
    else
    {
    std::cout << OK_PRINT << "DEBUG Disabled: NO validation layers\n";
    create_info.enabledLayerCount = 0;
    }


    vkCreateInstance( &create_info,
              nullptr,
              &dev.instance );

    return false;
}

bool VulkanDeviceInit::check_layer_support( std::vector<const char*> layers )
{
    std::vector<VkLayerProperties> supported_layers = get_supported_layers();

    for( auto wanted_layer : layers )
    {
    bool found = false;
    for( auto supp_layer : supported_layers )
        if( std::string(supp_layer.layerName) == wanted_layer )
        found = true;

    if( !found )
    {
        std::cout << BAD_PRINT << "ERROR Could not find layer: " << wanted_layer << "\n";
        return false;
    }
    }

    return true;
}

bool VulkanDeviceInit::check_extension_support( std::vector<const char*> extensions )
{
    std::vector<VkExtensionProperties> supported_extensions = get_supported_extentions();

    for( auto ext : extensions )
    {
    bool found = false;
    for( auto supp_ext : supported_extensions )
        if( std::string(supp_ext.extensionName) == ext )
        found = true;

    if( !found )
    {
        std::cout << BAD_PRINT << "ERROR Could not find extension: " << ext << "\n";
        return false;
    }
    }

    return true;
}

QueFamilyInfo VulkanDeviceInit::que_family_info( VulkanDevice& dev )
{
    QueFamilyInfo out;

    uint32_t q_fam_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev.physical_device, &q_fam_count, nullptr);
    std::vector<VkQueueFamilyProperties> q_fams(q_fam_count);
    vkGetPhysicalDeviceQueueFamilyProperties(dev.physical_device, &q_fam_count, q_fams.data());

    out.supports_graphics = false;
    out.supports_present = false;

    for( int i = 0; i < q_fams.size(); i++ )
    {
    auto q = q_fams[i];

    // graphicss
    if ( q.queueFlags & VK_QUEUE_GRAPHICS_BIT &&
         out.supports_graphics == false )
    {
        out.graphics_family_index = i;
        out.supports_graphics = true;
    }

    // presentation
    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR( dev.physical_device, i, dev.surface, &present_support );
    if( present_support &&
        out.supports_present == false )
    {
        out.present_family_index = i;
        out.supports_present = true;
    }

    }

    return out;
}

void VulkanDeviceInit::create_logical_device( VulkanDevice& dev, const VulkanDevicePreferences& pref )
{
    QueFamilyInfo selected_q_fam = que_family_info( dev );
    float q_prio = 1.0f;

    std::cout << OK_PRINT << "Graphics queue index: " << selected_q_fam.graphics_family_index << "\n";
    std::cout << OK_PRINT << "Present queue index: " << selected_q_fam.present_family_index << "\n";

    std::set<uint32_t> unique_que_index;
    unique_que_index.insert( selected_q_fam.graphics_family_index );
    unique_que_index.insert( selected_q_fam.present_family_index );

    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    for( auto index : unique_que_index )
    {
    VkDeviceQueueCreateInfo queue_create_info{};
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &q_prio;
    queue_create_infos.push_back( queue_create_info );

    std::cout << OK_PRINT << "Creating queue index: " << index << "\n";
    }

    VkPhysicalDeviceFeatures device_features{};
    device_features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = queue_create_infos.size();
    create_info.enabledExtensionCount = device_extensions.size();
    create_info.ppEnabledExtensionNames = device_extensions.data();
    create_info.pEnabledFeatures = &device_features;



    dev.queue_fam_info = selected_q_fam;
    VkResult res = vkCreateDevice(dev.physical_device, &create_info, nullptr, &dev.logical_device);

    // get the graphics queue
    vkGetDeviceQueue( dev.logical_device,
              dev.queue_fam_info.graphics_family_index,
              0,
              &dev.graphics_queue );

    // get the present queue
    vkGetDeviceQueue( dev.logical_device,
              dev.queue_fam_info.present_family_index,
              0,
              &dev.present_queue );


    if( res == VK_SUCCESS )
    std::cout << OK_PRINT << "Logical Device Created\n";
    else
    std::cout << BAD_PRINT << "Logical Device Failed to create\n";
}

void VulkanDeviceInit::wait_idle( VulkanDevice& dev )
{
    vkDeviceWaitIdle(dev.logical_device);
}

bool VulkanDeviceInit::clean( VulkanDevice& dev )
{

    std::cout << OK_PRINT << "cleanup\n";

    vkDestroyDevice(dev.logical_device, nullptr);
    DestroyDebugUtilsMessengerEXT( dev.instance, dev.debug_messenger, nullptr );
    vkDestroySurfaceKHR(dev.instance, dev.surface, nullptr);
    vkDestroyInstance(dev.instance, nullptr);
    return true;
}
