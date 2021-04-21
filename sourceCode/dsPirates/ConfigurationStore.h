/**@file */
#include "stdafx.h"
#pragma once

#include <map>

#include "Constants.h"
#include "EasySingleton.h"

typedef std::map<std::string, std::string>::const_iterator property_iterator;

/**This singleton contains all the configuration parameters.
Parameters are read from a from a file.
Debug values are still hardcoded for test pourposes and they override the file settings.*/
class ConfigurationStore {
	SINGLETON(ConfigurationStore);
public:
   std::string get(const std::string);
   property_iterator getAll();
   property_iterator getEnd();
   void overrideSetting(std::string, std::string);
private:
	ConfigurationStore(); //Singleton.

	///Key names for this map are defined in Constants.h (so that we use the same values everywhere)
	std::map<std::string, std::string> properties;

};

/**This is a shortcut to abbreviate  ConfigurationStore::getInstance()->get(...);*/
#define CONFIGURATION(p) ConfigurationStore::getInstance()->get(p)