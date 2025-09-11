#include "Input/MWInputHandler.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AMWInputHandler::AMWInputHandler(const FObjectInitializer& ObjectInitializer)
{
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AMWInputHandler::RemoveBindInputAction(const FInputBindingHandle& BindingToRemove)
{
	UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

	if (!mwic)
	{
		return;
	}

	mwic->RemoveBinding(BindingToRemove);
}

void AMWInputHandler::ClearBindingsForObject(UObject* InOwner)
{	
	UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

	mwic->ClearBindingsForObject(InOwner);	
}

void AMWInputHandler::BeginPlay()
{
	Super::BeginPlay();

	EnableInput(UGameplayStatics::GetPlayerController(this, 0));
}
