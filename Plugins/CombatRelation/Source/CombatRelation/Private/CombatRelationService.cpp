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

    // 優先順位1：Developer Settingsから読み込み
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

    // 優先順位2：Configファイルから読み込み（フォールバック）
    if (!LoadedAsset && !FallbackMatrixPath.IsNull())
    {
        LoadedAsset = Cast<UFactionRelationDataAsset>(FallbackMatrixPath.TryLoad());
        if (LoadedAsset)
        {
            UE_LOG(LogTemp, Log, TEXT("Combat Relation: Loaded from Config fallback: %s"), *FallbackMatrixPath.ToString());
        }
    }

    // 読み込んだアセットを設定
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
// デバッグ用コンソールコマンド：CombatRelationマトリックスを再読み込み
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
