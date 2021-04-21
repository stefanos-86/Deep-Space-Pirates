#Example of the settings file imported by the script.
#Fill your settings "once and forever" and keep the real script up to date.
#DO NOT COMMIT YOUR PERSONAL DATA IN THIS FILE - you should create a local copy outside the version control,
#call it dspBuilderPersonalSettings, put it in the same folder where you run the script and fill your data.
#Once this set up is ready you can check out the latest version of the dspBuilder and keep your settings.
#See below for the meaning of the parameters.

class PersonalBuildSettings:
    def __init__(self):
        print "Build personal settings"
        self.baseRepo = "<FILL YOUR PATH>\\tfsChekout\\deepspacepirates\\"
        self.releaseDestination = "."
        self.compileDirectory = "<FILL YOUR PATH>\\tfsChekout\\deepspacepirates\\dsPirates\\Release"
        self.buildDirectory = "<FILL YOUR PATH>\\ogre\\OgreSDK_vc10_v1-7-2\\bin\\release"
        self.runtimeDirectory = "<FILL YOUR PATH>\\Microsoft Visual Studio 10.0\\VC\\redist\\x86\\Microsoft.VC100.CRT"
        self.silent = True
        print "Personal settings ok"
    def test(self):
        print "The settings where really imported." #I had a fat lot of problems in having file "A" use a class in file "B":
print "The settings might have been imported."

        
# baseRepo - Root folder of the repository. Where you did the checkout.
#releaseDestination: where do you want to build the release package. A subfolder is created. "." is a valid path to say "where the script itself is".
#compileDirectory - Folder in which to look for dsPirates.exe
#buildDirectory - Folder where the program libraries and configuration files are.
#runtimeDirectory - Where to take the C++ runtime
#silent - True means "don't ask confirmation before potentially lethal operations" like deleting a folder. Set to false if you don't trust the script.

