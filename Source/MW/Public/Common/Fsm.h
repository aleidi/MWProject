#pragma once

// Include
#include "CoreMinimal.h"
#include "Common/FsmState.h"
#include "MWLogChannels.h"

// Forward Declare
template<typename T>
class FFsmState;

// Define
template<typename T>
class IFsm
{
public:
	virtual FName GetName() const = 0;
	virtual T* GetOwner() const	= 0;
	virtual int32 GetFsmStateCount() const = 0;
	virtual float GetCurrentStateTime() const = 0;
	virtual bool IsRunning() const = 0;
	virtual bool IsDestroyed() const = 0;
	virtual FFsmState<T>* GetCurrentState() const = 0;
	virtual FFsmState<T>* GetLastState() const = 0;
	virtual FFsmState<T>* GetState(FName) const = 0;
	virtual void Start(FName) = 0;
	virtual bool HasState(FName) = 0;
	virtual void ChangeState(FName) = 0;
	virtual void ChangeToLastState() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void ShutDown() = 0;
};

/*
 * @class FFsm
 * 
 * @brief The basic class of the fsm.
 *
 * @note T is the type of the fsm owner.
 */
template<typename T>
class FFsm : public TSharedFromThis<FFsm<T>>, public IFsm<T>
{
public:
	FFsm() = delete;

	template<typename... StateTypes>
	FFsm(FName InName, T* InOwner, StateTypes... InStates)
	: Name(InName), Owner(InOwner)
	{
		checkf(Owner != nullptr, TEXT("FSM owner is invalid"));

		AddState(InStates...);

		CurrentStateTime = 0.f;
		CurrentState = nullptr;
		LastState = nullptr;
		bDestroyed = false;
	}

	virtual ~FFsm() 
	{
		if (!bDestroyed)
		{
			ShutDown();
		}

		if (Owner != nullptr)
		{
			Owner = nullptr;
		}
	}

	virtual FName GetName() const override
	{
		return Name;
	}

	virtual T* GetOwner() const override
	{ 
		return Owner;
	}

	virtual int32 GetFsmStateCount() const override
	{ 
		return States.Num();
	}

	virtual float GetCurrentStateTime() const override 
	{ 
		return CurrentStateTime; 
	}

	virtual bool IsRunning() const override 
	{ 
		return CurrentState != nullptr;
	}

	virtual bool IsDestroyed() const override
	{
		return bDestroyed;
	}

	virtual FFsmState<T>* GetCurrentState() const override 
	{ 
		return CurrentState;
	}

	virtual FFsmState<T>* GetLastState() const override
	{ 
		return LastState;
	}

	virtual FFsmState<T>* GetState(FName StateName) const override
	{
		for (auto& state : States)
		{
			if (state->GetName() == StateName)
			{
				return state.Get();
			}
		}

		return nullptr;
	}

	virtual void Start(FName StateName) override
	{
		if (IsRunning())
		{
			UE_LOG(LogMWCommon, Warning, TEXT("FSM has aleady run."));
		}

		auto* state = GetState(StateName);
		if (!state)
		{
			UE_LOG(LogMWCommon, Warning, TEXT("FSM[%s] can not start state \"%s\" which is not exist."), *Name.ToString(), *state->GetName().ToString());
		}

		CurrentStateTime = 0.f;

		CurrentState = state;

		CurrentState->OnEnter();
	}

	virtual bool HasState(FName StateName) override
	{
		return GetState(StateName) != nullptr; 
	}

	virtual void ChangeState(FName StateName) override
	{
		ChangeState(GetState(StateName));
	}

	virtual void ChangeToLastState() override 
	{
		ChangeState(LastState);
	}

	virtual void Update(float DeltaTime) override
	{
		if (nullptr == CurrentState)
		{
			return;
		}

		CurrentStateTime += DeltaTime;

		CurrentState->OnUpdate(DeltaTime);
	}

	virtual void ShutDown() override
	{
		if (CurrentState != nullptr)
		{
			CurrentState->OnLeave(true);
			CurrentState = nullptr;
			CurrentStateTime = 0.f;
		}

		for (auto& state : States)
		{
			state->OnDestroy();
			state->SetFSM(nullptr);
			state->SetIndex(INDEX_NONE);
		}

		LastState = nullptr;

		States.Empty();

		bDestroyed = true;
	}

private:
	template<typename T, typename... ParamTypes>
	void AddState(T& State, ParamTypes&... RestStates)
	{
		checkf(State != nullptr, TEXT("FSM State is invalid."));

		States.Emplace(MoveTemp(State));

		int32 lastIndex = States.Num() - 1;

		States[lastIndex]->SetFSM(this);
		States[lastIndex]->SetIndex(lastIndex);
		States[lastIndex]->OnInit();

		AddState(RestStates...);
	}

	void AddState() {}

	void ChangeState(FFsmState<T>* NewState)
	{
		if (NewState)
		{
			CurrentState->OnLeave(false);

			CurrentStateTime = 0.f;

			LastState = CurrentState;

			CurrentState = NewState;

			CurrentState->OnEnter();
		}
		else
		{
			UE_LOG(LogMWCommon, Warning, TEXT("State[%s] to change is not found"), *(NewState->GetName().ToString()));
		}
	}

protected:
	/* Name of the state machine. */
	FName Name;

	/* Owner of the state machine. */
	T* Owner = nullptr;

	/* Is the state machine destroyed. */
	bool bDestroyed = false;

	/* How long current state is running. */
	float CurrentStateTime = 0.f;

	FFsmState<T>* CurrentState = nullptr;

	FFsmState<T>* LastState = nullptr;

	TArray<TSharedPtr<FFsmState<T>>> States;
};