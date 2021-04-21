/** @file
Logging system.

Could be very useful if for any reason we can not use the console output anymore.
Obvious case: the release version!
*/
#pragma once



class Logger
{
public:
   static void InitLogging(const char* appName);
   static void ForceLogInfo(const char* message);
};

