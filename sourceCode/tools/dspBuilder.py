# -*- coding: cp1252 -*-
#This script should help you build a release for Deep Space Pirates.
#Fill in the below data and let it build things for you. Remember to escape \ to \\ in paths.

import os            #File operations
import stat          #File permissions
import shutil        #Delete folder and all content
import sys           #Modify the pythonpath to pull in the settings

#Parameter that changes from user to user.
from dspBuilderPersonalSettings import PersonalBuildSettings
#reload(dspBuilderPersonalSettings) #My python interpreter sternly refurese to import the class without this.

#Nothing after this line should be edited for a release.

#Folders where it is expected to find the stuff you need.
gRepoFolders = {"models":"art\\models",
                "textures":"art\\materials\\textures",
                "audio":"art\\audio",
                "fonts":"art\\fonts",
                "materials":"art\\materials\\scripts",
                "particles":"art\\particles",
                "license":"docs\\Licenses",
                "physics":"art\\physics",
                "artRoot":"art",
                "readmeFile":"docs\\readme.txt"
               }

#Folders where the stuff is supposed to go in the distribution.
gReleaseFolders = {"license":"docs",
                   "program":"dsPirates\\game",
                   "artTree":"media\\dsp"
                  }
#Data that changes from programmer to programmer.
gSettings = PersonalBuildSettings()
gSettings.test()
#This function list the files with the given extension in the folder.
#This is useful for things that we don't know how to automatically export yet.
def ListManualExports(folder, extension):
    print extension + " files that require manual export:"
    os.chdir(folder)
    for currentFile in os.listdir("."):
        if currentFile.endswith(extension):
            print os.path.abspath(currentFile)
    print #newline

#Prints the message and ask the user to say Y or N.
#Returns the user answer, or true in silent mode.
def UserConfirm(message):
    print message + "[y,n]?"
    if gSettings.silent == False:
        userReply = raw_input()
        return userReply == "y"
    else:
        print "Silent mode, auto-answered y."
        return True

#Removes the read only attribute to everything it can find.
def MakeWritable(root):
    for dirpath, dirnames, filenames in os.walk(root):
        for filename in filenames:
            pathForChange = os.path.join(dirpath, filename)
            if pathForChange != root:
                os.chmod(pathForChange,stat.S_IWRITE)
        
#Delete the folder and recreates it.
#May ask to confirm depending on gSilent.
def ResetFolder(path):
    print "Resetting " + os.path.abspath(path)
    if (UserConfirm("Delete " + os.path.abspath(path))):
        if (os.path.exists(path)):
            if (os.path.isdir(path)):
                MakeWritable(path) #Files from the repo are read only. Reset permission.
                #Now erase the files.
                shutil.rmtree(path)
            else:
                raise Exception (str(path) + " is not a directory")
        os.mkdir(path, stat.S_IWRITE)
        os.chmod(path,stat.S_IWRITE)

#Copies file from src to dest if the extension matches ext
#Create dest if it does not exists.
def CopyContentByExtension(src, dest, ext):
    assert os.path.isdir(src)
    assert 2 < len(ext) and len(ext) < 5
    #Create the directory if needed.
    if not os.path.isdir(dest):
        os.mkdir(dest)
    assert os.path.isdir(dest)
    files = os.listdir(src)
    for filename in files:
        if filename.endswith(ext):
            print "Extension match: copy " + filename
            fullSource = os.path.join(src, filename)
            shutil.copy(fullSource, dest)
    

#Main procedure
print "Creation of a dsp release started."
print #Newline

#Create the destination folder.
print "Create base folder at " + gSettings.releaseDestination
releaseBase = os.path.join(gSettings.releaseDestination, "dsp")
ResetFolder(releaseBase)

#Copy the game executable and the support libraries.
print "Copy program folder (libs and config) from " + gSettings.buildDirectory
programDestination = os.path.join(releaseBase, gReleaseFolders["program"])
shutil.copytree(gSettings.buildDirectory, programDestination) #Also creates the folder

print "Copy program main exe from " + gSettings.compileDirectory
programFileName = "dsPirates.exe"
programFileSource = os.path.join(gSettings.compileDirectory, programFileName)
programFileDest = os.path.join(programDestination, programFileName)
shutil.copyfile(programFileSource, programFileDest)

#Replace the log files with empty ones (delete/recreate).
print "Creating empty logs."
logList = ["dsPiratesLog.txt", "ogre.log"]
if(UserConfirm("Delete old logs?")):
   for logfile in logList:
       fullLogPath = os.path.join(programDestination, logfile)
       os.remove(fullLogPath)
       newFile = open(fullLogPath, "w")
       newFile.close()

#Copy the license
licenseSource = os.path.join(gSettings.baseRepo, gRepoFolders["license"])
print "Copy license from " + licenseSource
licenseDestination = os.path.join(releaseBase, gReleaseFolders["license"])
shutil.copytree(licenseSource, licenseDestination)

#Copy the C++ runtime
print "Copy C++ runtime from " + gSettings.runtimeDirectory
files = os.listdir(gSettings.runtimeDirectory) #TODO: make a "copy all files" out of this?
for fileName in files:
    fullPath = os.path.join(gSettings.runtimeDirectory, fileName)
    if (os.path.isfile(fullPath)):
        shutil.copy(fullPath, programDestination)

#Copy what can be copied "as is" from the art.
#This takes everything, even "experimantal" stuff left in the repo. TODO: how to take only what is needed?
artsAsIs = []
artsAsIs.append(gRepoFolders["audio"])
artsAsIs.append(gRepoFolders["fonts"])
artsAsIs.append(gRepoFolders["materials"])
artsAsIs.append(gRepoFolders["particles"])
artsAsIs.append(gRepoFolders["physics"])
print "Copy resources that can be just copied: " + str(artsAsIs)
for resource in artsAsIs:
    resourceSource = os.path.join(gSettings.baseRepo, resource)
    resourceDestination = os.path.join(releaseBase, resource)
    shutil.copytree(resourceSource, resourceDestination)

print "Copy resources that are intermixed with source resources. Filter by extension."
#TODO: It would be nice to automate the exporter (e.g. calling a script from the
#blender command line). By now we keep the "ready for use" resources in the
#same tree of the "sources".
print "Copy .mesh models."
modelsSource = os.path.join(gSettings.baseRepo, gRepoFolders["models"])
modelsDestination = os.path.join(releaseBase, gRepoFolders["models"])
CopyContentByExtension(modelsSource, modelsDestination, "mesh")
print "Copy .png textures."
texturesSource = os.path.join(gSettings.baseRepo, gRepoFolders["textures"])
texturesDestination = os.path.join(releaseBase, gRepoFolders["textures"])
CopyContentByExtension(texturesSource, texturesDestination, "png")

#At this point the art is copied from the repo.
#Place it in the expected folder.
finalArtDestination = os.path.join(releaseBase, gReleaseFolders["artTree"])
artCurrentPosition = os.path.join(releaseBase, gRepoFolders["artRoot"])
MakeWritable(artCurrentPosition) #otherwise it cannot be deleted (the move seems to be a copy+delete)
shutil.move(artCurrentPosition, finalArtDestination)

#Copy the readme file.
readmeSource = os.path.join(gSettings.baseRepo, gRepoFolders["readmeFile"])
readmeDest = os.path.join(releaseBase, "README.TXT")
shutil.copy(readmeSource, readmeDest)

#TODO: it there any "final test" we can do to ensure the integrity of the release?
#Missing files etc. etc.

#Finally, warn the user of steps that he has to do manually.
#(This kind of export will in the future be automated).
print #newline
print "MANUAL OPERATIONS"
print "Test the release"
print "Tag the repo to record the release."
print "Zip the release (use a meaningful name)."
print "Upload the release to sourceferge."
