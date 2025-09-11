#pragma once

// Include Header
#include "GameFramework/Actor.h"
#include "MWInputComponent.h"
#include "MWInputHandler.generated.h"

// Forward Declare

// Macro

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
	
public:
	AMWInputHandler(const FObjectInitializer& ObjectInitializer);

	template<class UserClass>
	FInputBindingHandle BindInputAction(const UInputAction* Action, ETriggerEvent TriggerEvent, UserClass* Object, void (UserClass::* Func)(const FInputActionValue&))
	{
		UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

		checkf(mwic != nullptr, TEXT("Check if the enhanced input system is enabled in project settings."));

		return mwic->BindAction(Action, TriggerEvent, Object, Func);
	}

	void RemoveBindInputAction(const FInputBindingHandle& BindingToRemove);

	void ClearBindingsForObject(UObject* InOwner);

protected:
	virtual void BeginPlay() override;

};