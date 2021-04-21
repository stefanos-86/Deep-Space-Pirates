/** @file
This file hosts the project-wide constants.
There is no point in using files for constants that appears only in code.
This file should contain only "project-wide" constants.*/
#include "stdafx.h"

#include "ActionName.h" //Support for key names.

#pragma once

/**Deep space constants namespace. Hosts all the configuration constants.*/
namespace dsc{
	const std::string DS_APPLICATION_NAME = "Deep Space Pirates - Version 0";
	const std::string PROPERTY_LOG_FILE_NAME = "log_out";
	const std::string LOG_IS_ENABLED = "log_enabled";  
	const std::string MAIN_CONFIG_FILE = "dsConfig.cfg";  

	//ogre specific
	const std::string DS_OGRE_CONFIG_FILE_NAME= "ogre_config_file";  //the DS prefix distinguish it from ogre defines.
	const std::string DS_OGRE_PLUGINS_FILE_NAME ="ogre_plugins_file";
	const std::string DS_OGRE_LOG_FILE_NAME ="ogre_log_file";
	const std::string DS_OGRE_RESOURCES_FILE_NAME ="ogre_res_file";
	const std::string DS_OGRE_SHOW_CONFIG_WINDOW ="ogre_conf_window";

	//boolean values for boolean properties
	const std::string YES= "yes";
	const std::string NO= "no";
};



	
