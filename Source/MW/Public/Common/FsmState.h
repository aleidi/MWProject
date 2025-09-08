#pragma once

// Include
#include "CoreMinimal.h"
#include "Common/fsm.h"

// Forward Declare
template<typename T>
class FFsm;

// Macro

class IFsmState
{
public:
	virtual void OnEnter() = 0;
	virtual void OnUpdate(float DeltaTime) = 0;
	virtual void OnLeave(bool bShutDown) = 0;
	virtual FName GetName() const = 0;
};

/*
 * @class FFsmState
 * 
 * @brief The basic class of the fsm state.
 *
 * @note T is the type of the fsm owner.
 */
template<typename T>
class FFsmState : public TSharedFromThis<FFsmState<T>>, public IFsmState
{
public:
	FFsmState() = delete;

	FFsmState(FName InName)
	: Name(InName)
	{}

	virtual ~FFsmState()
	{
		Owner = nullptr;
		Fsm = nullptr;
	}

	virtual void OnInit() {}
	virtual void OnEnter() override {}
	virtual void OnUpdate(float DeltaTime) override {}
	virtual void OnLeave(bool bShutDown) override {}
	virtual void OnDestroy() {}
	virtual FName GetName() const { return TEXT("FFsmState"); }

	void ChangeState(FName StateName)
	{
		checkf(Fsm != nullptr, TEXT("Fsm cached in fsm state is not valid."));

		Fsm->ChangeState(StateName);
	}

	T* GetOwner() const 
	{
		return Fsm ? Fsm->GetOwner() : nullptr;
	}

	void SetFSM(FFsm<T>* InFsm)
	{
		Fsm = InFsm;
	}

	void SetOwner(T* InOwner)
	{
		Owner = InOwner;
	}

	void SetIndex(int32 NewValue)
	{
		Index = NewValue;
	}

protected:
	/* The name of the Fsm state. */
	FName Name;

	/* The fsm the state runs on. */
	FFsm<T>* Fsm = nullptr;

	int32 Index = INDEX_NONE;

	T* Owner = nullptr;
};