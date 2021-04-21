Many of the object that compose the game engine are unique: a single object manager, a single facade... 
Some sintactic sugar to implement again and again the singleton pattern is then welcome.

Any class can be transformed into a singleton using the macros in the "EasySingleton.h" file. In the header file for yur class just add the SINGLETON macro

{{
 class ClassName { 
     SINGLETON(ClassName)
 public:
     …
}}

In the definition file, after the inclusions, use the SINGLETON_DEF:

{{
 #include “Something.h”
 SINGLETON_DEF(ClassName)
 void ClassName::method(...){...}
 ...
}}

One the macros are expanded the class will be provided of the ClassName::getInstance() method and of the ClassName::destroy()
method. The second one can be used to destroy the singleton instance (it calls delete on the instance). This is necessary as C++ does
not provide any garbage collection system to take care of the instance deletion. The macros do not provide constructors, as the
instance initialization may be not trivial. The user is therefore responsible for the enforcement the constructors visibility (a “pure”
singleton would use private constuctors).

This part should have been implemented using templates, but at the time of the implementation too little template experience was available (and too much C preprocessor bad habits were stronger).