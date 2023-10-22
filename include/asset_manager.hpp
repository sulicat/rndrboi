#pragma once

namespace rndrboi
{
    class AssetManager
    {
    public:

	static AssetManager* Instance();

    private:
	AssetManager(){}
	~AssetManager(){}

	static AssetManager* instance;
    };
};
