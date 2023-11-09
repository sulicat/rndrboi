#include "vulkan_wrappers/vulkan_device.hpp"

#define ATTENTION_PRINT (A_BLUE "[DEVICE] " A_RESET)
#define OK_PRINT (A_YELLOW "[DEVICE] " A_RESET)
#define BAD_PRINT (A_RED "[DEVICE] " A_RESET)


using namespace rndrboi;

Device* Device::instance = NULL;

Device* Device::Instance()
{
    if (!instance)
    {
        std::cout << OK_PRINT << "Created Device Instance\n";
        instance = new Device;
    }
    return instance;
}

//----------------------------------------------------------------------------------------------------

void Device::init()
{
    // choose device
    rndrboi::VulkanDevicePreferences dev_preferences{};
    dev_preferences.graphics        = true;
    dev_preferences.present         = true;
    dev_preferences.limit_one_queue = true;
    dev_preferences.debug           = true;
    dev_preferences.print_info      = true;
    device = VulkanDeviceInit::init( dev_preferences );
}
