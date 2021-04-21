/**@file */
#pragma once
/**This project is full of singletons. For this reason the code to add the methods is factored in here. The macro provided by this file add
"automatically" the usual singleton consructs. This reduces copy&paste code.

Just include this file and put SINGLETON(NameOfYourClass) in the class declaration in the header file, and SINGLETON_DEF(NameOfYourClass) at the beginning of 
the ".cpp" file, after inclusions.


NONE of the macros require a ';' at the end: use SINGLETON_SOMETHING(Class) , do NOT put ; at the end.
*/

/**Declaration for the class header file. This macro declares the instance variable, the getInstance and the Destroy methods.
Use at the beginning of the class: Class{ SINGLETON(Class) private::... It contains its oun public and private declarations.
It does NOT provide a constructor (unfortunately at least one singleton requires it to be available and public, for testability).
 */
#define SINGLETON(classname) private: static classname* instance; public: static classname* getInstance(); static void destroy();

/**Creates the tipical getInstance method:

 classname* classname::getInstance(){
	if(classname::instance==NULL){
		classname::instance=new classname();
	}
	return classname::instance;}
	
	Do NOT use togheter with SINGLETON_METHODS*/
#define SINGLETON_GET_INSTANCE(classname) classname* classname::getInstance(){if(classname::instance==NULL){classname::instance=new classname();} return classname::instance;}

/**C++ has no garbage collection. Therefore there must be a way to de-allocate the singletons.
Do NOT use togheter with SINGLETON_METHODS.
This macro expands to the following method: 

void classname::destroy(){
	if(classname::instance!=NULL){
		delete classname::instance;
	}
	classname::instance=NULL; //will trigger the re-creation of the instance at the next get instance
}
*/
#define SINGLETON_DESTROY(classname) void classname::destroy(){if(classname::instance!=NULL){delete classname::instance; classname::instance=NULL;}}


/**This replace the declaration of the instance variable in the .cpp file. Put this macro on top of the file.
Do NOT use togheter with SINGLETON_METHODS*/
#define SINGLETON_INSTANCE_DEF(classname) classname* classname::instance=NULL; 

/**To create the instance, the getInstance and the destroy() in a single shot, use this macro. This macro should be placed in the .cpp file.
Do NOT use togheter with SINGLETON_GET_INSTANCE or SINGLETON_DESTROY or SINGLETON_INSTANCE_DEF*/
#define SINGLETON_DEF(classname2)  SINGLETON_INSTANCE_DEF(classname2) SINGLETON_GET_INSTANCE(classname2) SINGLETON_DESTROY(classname2)




/**Provides a shortcut to access a class instance, but requires the user to include EasySingleton.*/
#define INST(classname) classname::getInstance()