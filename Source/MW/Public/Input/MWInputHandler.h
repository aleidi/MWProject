#pragma once

// Include Header
#include "GameFramework/Actor.h"
#include "MWInputComponent.h"
#include "MWInputHandler.generated.h"

// Forward Declare

// Define

/*
 * @class AMWInputHandler
 * 
 * @brief
 *
 * @note
 */
UCLASS()
class AMWInputHandler : public AActor
{
	GENERATED_BODY()
	
	typedef TMap<const UInputAction*, uint32> ActionBindingMap;

public:
	AMWInputHandler(const FObjectInitializer& ObjectInitializer);

	template<class UserClass, typename FuncType, typename... VarTypes>
	void BindInputAction(const UInputAction* Action, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, VarTypes... Params)
	{
		UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

		checkf(mwic != nullptr, TEXT("Check if the enhanced input system is enabled in project settings."));

		uint32 handle = mwic->BindAction(Action, TriggerEvent, Object, Func, Params...).GetHandle();

		BindActionhandleWithObject(Object, Action, handle);
	}

	bool RemoveBindingInputAction(uint32 BindingToRemove);

	bool RemoveBindingInputAction(const void* Object, const UInputAction* Action);

	void ClearBindingsForObject(UObject* InOwner);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/* Cache the InputBindingHandle with the object and action. */
	void BindActionhandleWithObject(const void* Object, const UInputAction* Action, uint32 Handle);
	
private:
	TMap<const void*, ActionBindingMap> BindingHandles;
};