#include "GameplayAbility/Ability/Battle/MWBattleMoveCommand.h"

UMWBattleMoveCommand::UMWBattleMoveCommand(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMWBattleMoveCommand::PostInitProperties()
{
	Super::PostInitProperties();

	// Populate AbilityTriggers from ActivateTag after properties have been initialized.
	AbilityTriggers.Empty();

	if (ActivateTag.IsValid())
	{
		FAbilityTriggerData triggerData;
		triggerData.TriggerTag = ActivateTag;
		AbilityTriggers.Emplace(triggerData);
	}
}

#if WITH_EDITOR
void UMWBattleMoveCommand::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property && PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UMWBattleMoveCommand, ActivateTag))
	{
		AbilityTriggers.Empty();

		if (ActivateTag.IsValid())
		{
			FAbilityTriggerData triggerData;
			triggerData.TriggerTag = ActivateTag;
			AbilityTriggers.Emplace(triggerData);
		}
	}
}
#endif