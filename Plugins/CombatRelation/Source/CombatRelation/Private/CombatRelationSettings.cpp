#include "CombatRelationSettings.h"
#include "FactionRelationDataAsset.h"

#if WITH_EDITOR
#include "Editor.h"

void UCombatRelationSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // Check if DefaultFactionRelationAsset was modified
    if (PropertyChangedEvent.Property && 
        PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCombatRelationSettings, DefaultFactionRelationAsset))
    {
        UE_LOG(LogTemp, Log, TEXT("Combat Relation Settings: DefaultFactionRelationAsset changed to: %s"), *DefaultFactionRelationAsset.ToString());

        // Force load the asset to validate it
        if (!DefaultFactionRelationAsset.IsNull())
        {
            UFactionRelationDataAsset* LoadedAsset = DefaultFactionRelationAsset.LoadSynchronous();
            if (LoadedAsset)
            {
                UE_LOG(LogTemp, Log, TEXT("Combat Relation Settings: Asset validated successfully"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Combat Relation Settings: Failed to load asset from path: %s"), *DefaultFactionRelationAsset.ToString());
            }
        }
    }
}
#endif

void UCombatRelationSettings::PostInitProperties()
{
    Super::PostInitProperties();

    // Preload asset in both editor and runtime
    // This ensures the asset is properly loaded and included in package builds
    if (!DefaultFactionRelationAsset.IsNull())
    {
        UFactionRelationDataAsset* LoadedAsset = DefaultFactionRelationAsset.LoadSynchronous();
        if (LoadedAsset)
        {
            UE_LOG(LogTemp, Log, TEXT("Combat Relation Settings: DefaultFactionRelationAsset preloaded: %s"), *DefaultFactionRelationAsset.ToString());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Combat Relation Settings: Failed to preload asset from path: %s"), *DefaultFactionRelationAsset.ToString());
        }
    }
}