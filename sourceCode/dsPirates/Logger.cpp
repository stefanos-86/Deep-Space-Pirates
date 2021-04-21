#include "stdafx.h"
#include "ConfigurationStore.h"
#include "Logger.h"

using namespace std;
using namespace dsc;

void Logger::InitLogging(const char* appName)
{
	auto filename = ConfigurationStore::getInstance()->get(PROPERTY_LOG_FILE_NAME);
	auto log_enabled = ConfigurationStore::getInstance()->get(LOG_IS_ENABLED);
	bool logIsEnabled = (log_enabled == "yes");
	google::SetLogDestination(google::GLOG_INFO, filename.c_str());
	// setting log destination to "" should not create corresponding log files
	// Note: INFO log file contains all logs (e.g. ERROR, WARNING)
	google::SetLogDestination(google::GLOG_ERROR, "");
	google::SetLogDestination(google::GLOG_WARNING, "");

	FLAGS_minloglevel = logIsEnabled ? 0 : 3;	// log everything
	
	google::InitGoogleLogging(appName);

	LOG(INFO) << "Logging Started to file " << filename;
	LOG(INFO) << "Log status is " << log_enabled;
}

void Logger::ForceLogInfo( const char* message )
{
	auto currentLevel = FLAGS_minloglevel;
	FLAGS_minloglevel=0;
	LOG(INFO) << "{FORCED ENTRY} " << message;
	FLAGS_minloglevel=currentLevel;
}
