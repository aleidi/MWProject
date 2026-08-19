#include "CombatRelationSettings.h"
#include "FactionRelationDataAsset.h"

#if WITH_EDITOR
#include "Editor.h"

void UCombatRelationSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    // DefaultFactionRelationAssetが変更されたか確認
    if (PropertyChangedEvent.Property && 
        PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(UCombatRelationSettings, DefaultFactionRelationAsset))
    {
        UE_LOG(LogTemp, Log, TEXT("Combat Relation Settings: DefaultFactionRelationAsset changed to: %s"), *DefaultFactionRelationAsset.ToString());

        // アセットを強制読み込みして検証
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

    // エディターとランタイムの両方でアセットを事前読み込み
    // これによりアセットが正しく読み込まれ、パッケージビルドに含まれることを保証
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