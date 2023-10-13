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

void VulkanAPI::init_default()
{
    std::cout << A_YELLOW << "[VAPI] " << A_RESET << "init default\n";

    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics		= true;
    dev_preferences.present		= true;
    dev_preferences.limit_one_queue	= true;
    dev_preferences.debug		= true;
    dev_preferences.print_info		= true;

    device_data = VulkanDeviceInit::init( dev_preferences );
    swapchain.create( device_data );

    std::cout << swapchain << "\n";
}

void VulkanAPI::cleanup()
{

    swapchain.clean();
    VulkanDeviceInit::clean( device_data );
}
