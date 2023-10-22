#include "asset_manager.hpp"
#include "utils.hpp"

#define ATTENTION_PRINT (A_BLUE "[ASSET MANAGER] " A_RESET)
#define OK_PRINT (A_YELLOW "[ASSET MANAGER] " A_RESET)
#define BAD_PRINT (A_RED "[ASSET MANAGER] " A_RESET)

using namespace rndrboi;

AssetManager* AssetManager::instance = NULL;

AssetManager* AssetManager::Instance()
{
    if (!instance){
	std::cout << OK_PRINT << "Asset Manager Created\n";
	instance = new AssetManager;
    }
    return instance;
}

