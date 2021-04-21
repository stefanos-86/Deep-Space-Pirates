#include "stdafx.h"

#include "SanityCheck.h"
#include "Constants.h"
#include "ConfigurationStore.h"
#include <iostream>
#include <fstream>
#include <direct.h>


using namespace std;


bool SanityCheck::runTests(){
	//RLOG("Running env. test");
	LOG(INFO) << "Running env. test";
	bool allOk=true;
	allOk=allOk && haveConfigFile(); //should be done before testFileReachability
	allOk= allOk &&  showAppPath();
	allOk = allOk && testFileReachability();
	//DSLOG("Running env. test - ALL DONE!");
	LOG(INFO) << "Running env. test - ALL DONE!";
	return allOk;
}

/**Try to open files and see if they are really there. Files to be checked are recognized because 
"file" is a substring of the property name.*/
bool SanityCheck::testFileReachability(){
	//RLOG("Running env. test - file reachability");
	LOG(INFO) << "Running env. test - file reachability";
	bool ret=true;
	property_iterator it = ConfigurationStore::getInstance()->getAll();
	property_iterator end = ConfigurationStore::getInstance()->getEnd();
	

	//look for files: the property name contains "file"
	while(it != end && ret==true){
		string pName= it->first;
		if(pName.find("file") != string::npos){
			string pValue=it->second;
			if(pValue.size() > 0){ //property may be empty
				try{
				 ifstream file (pValue);
				 ret = file.good();
				 if(!ret){
					 //RLOG("Problem with file " + pValue);
					LOG(ERROR) << "Problem with file " << pValue;
				 }
				 file.close();
				}catch(exception e){
					std::cout<<"Problem with file " + pValue<<std::endl;
					//RLOG("Problem with file " + pValue);
					LOG(ERROR) << "Problem with file " << pValue;
				}
			}
		}
		it++;
	}
	return ret;
}


bool SanityCheck::showAppPath(){
	//RLOG("executable path given by cd: ");
	LOG(INFO) << "executable path given by cd: ";
	system("cd");
	return true;
}


//attempt to open the basic config file-does not check the content.
bool SanityCheck::haveConfigFile(){
	//RLOG("haveConfigFile");
	LOG(INFO) << "haveConfigFile";

	ifstream file (dsc::MAIN_CONFIG_FILE); 
	bool fileFound = file.good();

	char path[255];
	_getcwd(path,254);

	if(fileFound == false){
		std::cout<<"Problem with main configuration file"<<std::endl;
		printf("%s\n", path);
		//RLOG("Problem with main configuration file");
		LOG(INFO) << "Problem with main configuration file";
	}
	return  fileFound;
} 