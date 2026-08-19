#pragma once

// インクルード
#include "MW.h"
#include "Common/fsm.h"

// 前方宣言
template<typename T>
class FFsm;

// 定義

class IFsmState
{
	virtual void OnEnter() = 0;
	virtual void OnUpdate(float DeltaTime) = 0;
	virtual void OnLeave(bool bShutDown) = 0;
	virtual FName GetName() const = 0;
};

/*
 * @class FFsmState
 * 
 * @brief FSM状態の基底クラスです。
 *
 * @note TはFSM所有者の型です。
 */
template<typename T>
class FFsmState : public TSharedFromThis<FFsmState<T>>, public IFsmState
{
	friend class FFsm<T>;

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

protected:
	void ChangeState(FName StateName)
	{
		checkf(Fsm != nullptr, TEXT("Fsm cached in fsm state is not valid."));

		Fsm->ChangeState(StateName);
	}

public:
	virtual FName GetName() const 
	{ 
		return Name; 
	}

	T* GetOwner() const 
	{
		return Fsm ? Fsm->GetOwner() : nullptr;
	}

	FFsm<T>* GetFsm() const 
	{
		return Fsm;
	}

	int32 GetIndex() const 
	{
		return Index;
	}

private:
	virtual void OnInit() {}
	virtual void OnEnter() {}
	virtual void OnUpdate(float DeltaTime) {}
	virtual void OnLeave(bool bShutDown) {}
	virtual void OnDestroy() {}

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
	/* FSM状態名。 */
	FName Name;

	/* この状態を実行するFSM。 */
	FFsm<T>* Fsm = nullptr;

	int32 Index = INDEX_NONE;

	T* Owner = nullptr;
};