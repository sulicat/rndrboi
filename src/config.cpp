#include "config.hpp"


using namespace rndrboi;

Config* Config::instance = NULL;

Config::Config(){}

Config::~Config(){}


Config* Config::Instance(){
    if (!instance){
	instance = new Config;
    }
    return instance;
}

