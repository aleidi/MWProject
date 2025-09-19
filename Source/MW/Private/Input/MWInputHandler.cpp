#include "Input/MWInputHandler.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AMWInputHandler::AMWInputHandler(const FObjectInitializer& ObjectInitializer)
{
	AutoReceiveInput = EAutoReceiveInput::Player0;
}

bool AMWInputHandler::RemoveBindingInputAction(uint32 BindingToRemove)
{
	UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);

	if (!mwic)
	{
		return false;
	}

	return mwic->RemoveBindingByHandle(BindingToRemove);
}

bool AMWInputHandler::RemoveBindingInputAction(const void* Object, const UInputAction* Action)
{
	auto* value = BindingHandles.Find(Object);

	if (!value)
	{
		return false;
	}
	
	auto* handle = value->Find(Action);

	if (!handle)
	{
		return false;
	}

	bool res = RemoveBindingInputAction(*handle);

	// Clear ActionBindingMap and BindingHandles if no more bindings for the object
	if (res)
	{
		value->Remove(Action);

		if (value->Num() == 0)
		{
			BindingHandles.Remove(Object);
		}
	}

	return true;
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

void AMWInputHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	BindingHandles.Empty();

	Super::EndPlay(EndPlayReason);
}

void AMWInputHandler::BindActionhandleWithObject(const void* Object, const UInputAction* Action, uint32 Handle)
{
	auto& key = BindingHandles.FindOrAdd(Object);

	key.Add(Action, Handle);
}
