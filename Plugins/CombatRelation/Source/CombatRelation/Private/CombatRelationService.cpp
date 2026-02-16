#include "CombatRelationService.h"
#include "FactionRelationDataAsset.h"
#include "CombatRelationSettings.h"

void UCombatRelationService::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    LoadMatrixFromSettings();
}

void UCombatRelationService::LoadMatrixFromSettings()
{
    UFactionRelationDataAsset* LoadedAsset = nullptr;

    // Priority 1: Load from Developer Settings
    const UCombatRelationSettings* Settings = GetDefault<UCombatRelationSettings>();
    if (Settings)
    {
        if (!Settings->DefaultFactionRelationAsset.IsNull())
        {
            LoadedAsset = Settings->DefaultFactionRelationAsset.LoadSynchronous();
            if (LoadedAsset)
            {
                UE_LOG(LogTemp, Log, TEXT("Combat Relation: Loaded from Developer Settings: %s"), *Settings->DefaultFactionRelationAsset.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Combat Relation: Failed to load asset from path: %s"), *Settings->DefaultFactionRelationAsset.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Combat Relation: DefaultFactionRelationAsset is not set in Developer Settings"));
        }
    }

    // Priority 2: Load from Config file (Fallback)
    if (!LoadedAsset && !FallbackMatrixPath.IsNull())
    {
        LoadedAsset = Cast<UFactionRelationDataAsset>(FallbackMatrixPath.TryLoad());
        if (LoadedAsset)
        {
            UE_LOG(LogTemp, Log, TEXT("Combat Relation: Loaded from Config fallback: %s"), *FallbackMatrixPath.ToString());
        }
    }

    // Set loaded assets
    if (LoadedAsset)
    {
        SetActiveMatrix(LoadedAsset);
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Combat Relation: No default matrix configured"));
}

void UCombatRelationService::SetActiveMatrix(UFactionRelationDataAsset* InDataAsset)
{
    ActiveMatrix = InDataAsset;
}

ECombatRelation UCombatRelationService::Query(const FGameplayTag& SelfFaction, const FGameplayTag& OtherFaction) const
{
    return ActiveMatrix ? ActiveMatrix->Query(SelfFaction, OtherFaction) : ECombatRelation::Neutral;
}

bool UCombatRelationService::IsRelationDataSet() const
{
	return ActiveMatrix != nullptr;
}

#if !UE_BUILD_SHIPPING
// Console command for debugging: reload combat relation matrix
static FAutoConsoleCommand CCmdReloadCombatRelation(
    TEXT("CombatRelation.Reload"),
    TEXT("Reloads the combat relation matrix from settings"),
    FConsoleCommandDelegate::CreateStatic([]()
    {
        if (GEngine && GEngine->GameViewport)
        {
            UWorld* World = GEngine->GameViewport->GetWorld();
            if (World && World->GetGameInstance())
            {
                if (UCombatRelationService* Service = World->GetGameInstance()->GetSubsystem<UCombatRelationService>())
                {
                    Service->LoadMatrixFromSettings();
                    UE_LOG(LogTemp, Log, TEXT("Combat Relation: Matrix reloaded"));
                }
            }
        }
    })
);
#endif
