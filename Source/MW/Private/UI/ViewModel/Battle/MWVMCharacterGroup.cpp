#include "UI/ViewModel/Battle/MWVMCharacterGroup.h"

#include "UI/ViewModel/Battle/MWVMCharacterStatus.h"

UMWVMCharacterGroup::UMWVMCharacterGroup()
{
}

UMWVMCharacterStatus* UMWVMCharacterGroup::GetMainCharacter() const
{
	return VMMainCharacter;
}

UMWVMCharacterStatus* UMWVMCharacterGroup::GetSubCharacter() const
{
	return VMSubCharacter;
}

void UMWVMCharacterGroup::OnInitialize()
{
	if (!VMMainCharacter)
	{
		VMMainCharacter = NewObject<UMWVMCharacterStatus>(this);
	}

	if (!VMSubCharacter)
	{
		VMSubCharacter = NewObject<UMWVMCharacterStatus>(this);
	}

	UObject* worldContextObject = GetWorldContextObject();

	if (VMMainCharacter && worldContextObject)
	{
		VMMainCharacter->Initialize(worldContextObject);
	}

	if (VMSubCharacter && worldContextObject)
	{
		VMSubCharacter->Initialize(worldContextObject);
	}
}

void UMWVMCharacterGroup::OnDeinitialize()
{
	if (VMMainCharacter)
	{
		VMMainCharacter->Deinitialize();
	}

	if (VMSubCharacter)
	{
		VMSubCharacter->Deinitialize();
	}
}
