#include "stdafx.h"
#pragma once

/**This class performs some run-time test on the configuration, to be sure that everything is OK.
Can be a great time saver to detect deplyment errors (and I know from work experience).*/
class SanityCheck
{
public:
	bool runTests();

private:
	bool testFileReachability();
	bool showAppPath();
	bool haveConfigFile();
};

