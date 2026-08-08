#include "UI/ViewModel/MWViewModelBase.h"

void UMWViewModelBase::Initialize(UObject* InWorldContextObject)
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		return;
	}

	if (!InWorldContextObject)
	{
		return;
	}

	if (bInitialized)
	{
		if (WorldContextObject == InWorldContextObject)
		{
			return;
		}

		Deinitialize();
	}

	WorldContextObject = InWorldContextObject;

	bInitialized = true;

	OnInitialize();
}

void UMWViewModelBase::Deinitialize()
{
	if (!bInitialized)
	{
		return;
	}

	OnDeinitialize();

	WorldContextObject = nullptr;

	bInitialized = false;
}

bool UMWViewModelBase::IsInitialized() const
{
	return bInitialized;
}

UObject* UMWViewModelBase::GetWorldContextObject() const
{
	return WorldContextObject.Get();
}
