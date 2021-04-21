#pragma once

/**Support class to represent the "death sentence" of a game item that can require to be deleted during a collision callback.
The client is supposed do specialize the class to provide the proper "demolition logic" for every object.*/
class GameItemRemover
{
public:
	/**The client must implement the object deletion logic here.*/
	virtual void postCollisionRemove() = 0;
};