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
	
	typedef TMap<const UInputAction*, FInputBindingHandle> ActionBindingMap;

public:
	AMWInputHandler(const FObjectInitializer& ObjectInitializer);

	template<class UserClass>
	void BindInputAction(const UInputAction* Action, ETriggerEvent TriggerEvent, UserClass* Object, void (UserClass::* Func)(const FInputActionValue&))
	{
		UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

		checkf(mwic != nullptr, TEXT("Check if the enhanced input system is enabled in project settings."));

		FInputBindingHandle handle = mwic->BindAction(Action, TriggerEvent, Object, Func);

		BindActionhandleWithObject(Object, Action, handle);
	}

	bool RemoveBindingInputAction(const FInputBindingHandle& BindingToRemove);

	bool RemoveBindingInputAction(const void* Object, const UInputAction* Action);

	void ClearBindingsForObject(UObject* InOwner);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/* Cache the InputBindingHandle with the object and action. */
	void BindActionhandleWithObject(const void* Object, const UInputAction* Action, const FInputBindingHandle& Handle);
	
private:
	TMap<const void*, ActionBindingMap> BindingHandles;
};