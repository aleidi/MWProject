#include "System/MWWorldSubsystem.h"
#include "Input/MWInputHandler.h"

void UMWWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!IsValid(InputHandler))
	{
		InputHandler = GetWorld()->SpawnActor<AMWInputHandler>(AMWInputHandler::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	}
}

void UMWWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();

	if (IsValid(InputHandler))
	{
		InputHandler->MarkAsGarbage();
	}
}