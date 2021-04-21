#include "stdafx.h"
#include "ConfigurationStore.h"
#include <fstream>

using namespace std;
using namespace dsc;



SINGLETON_DEF(ConfigurationStore)

//Return a copy of the string without leading and trailing spaces.
std::string trimSpaces(const std::string& s){
	const std::string WHITE (" \t\f\v\n\r");
	size_t beginText=s.find_first_not_of(WHITE);
	size_t endText=s.find_last_not_of(WHITE);
	return s.substr(beginText, endText+1);
}

/**Read the config file for properties.
Will not check for exceptions. If it throws, there is nothing we can do. No properties, no dsPirates.*/
void parseInputFile(std::map<std::string, std::string>& properties, const std::string& filename){
	ifstream configFile(filename);
	std::string currentLine;
	
	const char COMMENT   = '#';
	const char SEPARATOR = '=';

	while(getline(configFile, currentLine)){
		//Strip away comments
		size_t commentStart=currentLine.find(COMMENT);
		if(commentStart != std::string::npos){
			currentLine=currentLine.substr(0, commentStart);
		}

		//Discard empty lines (A line may be empty after deleting comments, or a blank in the file).
		if(currentLine.length()==0)
			continue;
		
		//Read the key and the value. Multi-word keys are allowed.
		std::string key;
		std::string value;

		size_t separatorPos = currentLine.find(SEPARATOR);
		if(separatorPos != std::string::npos){
			key=currentLine.substr(0, separatorPos);
			key=trimSpaces(key);
			if(separatorPos+1 >= currentLine.length()){
				//Key with empty value,	 just store it.
				std::string empty="";
				properties.insert(std::make_pair(key, empty));
			}else{
				//Normal entry.
				value=currentLine.substr(separatorPos+1, currentLine.length());
				value=trimSpaces(value);
				properties.insert(std::make_pair(key, value));
			}
		}else{
			//Line without '=', but not empty nor comment. Just skip it.
			LOG(WARNING) << "Parsing of config file - Line without '=', but not empty nor comment.";
		}
	} //line loop.
}

/**Constructor that loads parameters from a file (dsConfig.cfg).*/
ConfigurationStore::ConfigurationStore(){


	/*Parse the file*/
	try{
		parseInputFile(this->properties, MAIN_CONFIG_FILE);
	}catch (exception e){
		//RLOG("Problem with property file.");
		LOG(ERROR) << "Problem with property file.";
		throw e; //nothing we can do
	}

	/*override the plugin setting with hardcoded debug property.*/
	#ifdef _DEBUG
		this->properties[DS_OGRE_PLUGINS_FILE_NAME]="dsp_plugins_d.cfg";
		this->properties[DS_OGRE_RESOURCES_FILE_NAME]="dsp_resources_d.cfg";
	#endif


}

std::string ConfigurationStore::get(const std::string propertyName){
	return this->properties[propertyName];
}


/** Returns an iterator to scroll the property map*/
property_iterator ConfigurationStore::getAll(){
	return this->properties.begin();
}

/**The iterator at the end of the map, to stop the searches*/
property_iterator ConfigurationStore::getEnd(){
	return this->properties.end();
}

/**To change settings at run time.
@param name The name of the property
@param value The new value*/
void ConfigurationStore::overrideSetting(string name, string value){
	this->properties[name]=value;
}

