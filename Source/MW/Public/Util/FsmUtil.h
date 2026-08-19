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

#define DECLARE_FSM_STATE_START_NOCONSTR(OwnerClass, FsmState) \
class FsmState : public FFsmState<OwnerClass> \
{ \

#define DECLARE_FSM_STATE_END() };

/*** 使用テンプレート ***
FSM Stateクラスの宣言にはDECLARE_FSM_STATE_STARTとDECLARE_FSM_STATE_ENDの使用を推奨します。
例として"ExampleState"クラスは次のように宣言します：
DECLARE_FSM_STATE_START(ExampleState)
// Constructorは追加で宣言しません。
// クラス本体をここに記述します。
public:
	void TestFunc();
	...
	...
	...
DECLARE_FSM_STATE_END

上記の宣言後、"new ExampleState<T>()"でFSM State Instanceを生成できます。

*** 注意 ***
FSM Stateを個別に宣言する場合は、ChangeState()で使用するためGetName()をOverrideし、
クラス名を返すことを推奨します。
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