#pragma once

#include "Common/Fsm.h"
#include "Common/FsmState.h"

#define DECLARE_FSM_STATE_START(OwnerClass, FsmState) \
class FsmState : public FFsmState<OwnerClass> \
{ \
public: \
	FsmState() \
	: FFsmState<OwnerClass>(TEXT(#FsmState)) \
	{ \
	} \
private:

#define DECLARE_FSM_STATE_END() };

/*** Use Template ***
Recommend to use the macro DECLARE_FSM_STATE_START and DECLARE_FSM_STATE_END to declare a Fsm State class.
For example, if you want to declare a class named "ExampleState", you can do like follow:
DECLARE_FSM_STATE_START(ExampleState)
// Don't declare any constructor function again.
// Put class Bod Here
public:
	void TestFunc();
	...
	...
	...
DECLARE_FSM_STATE_END

With the declaration above, you can use "new ExampleState<T>()" to define a Fsm State instance.

*** Notice ***
If you want to declare a Fsm State by yourself, it's recommended to override GetName() function
and return the class name since the GetName() will be used for ChangeState().
*/

class IStandaloneFsm
{
public:
	template<typename T, typename... StateTypes>
	static TSharedPtr<FFsm<T>> Create(FName InName, T* InOwner, StateTypes... InStates)
	{
		return MakeShared<FFsm<T>>(InName, InOwner, InStates...);
	}
};