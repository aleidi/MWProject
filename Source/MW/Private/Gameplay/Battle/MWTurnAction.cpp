#include "Gameplay/Battle/MWTurnAction.h"
#include "Input/MWInputUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Input/MWInputConfig.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
#include "Gameplay/MWGameplayTags.h"
UE_DISABLE_OPTIMIZATION

void UMWTurnAction::SetActionUnits(const TArray<FMWTeam>& InUnits)
{
    ActionUnits = InUnits;
}

void UMWEnemyTurnAction::Init()
{
    // TODO : init action points of align dynamically
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Enemy Turn"));
}

void UMWEnemyTurnAction::Update(const FMWTurnActionData& InInfo, bool& OutIsFin)
{
	OutIsFin = false;

    time += 0.0167f;
    if (time >2.f)
    {
        OutIsFin = true;
    }
}

void UMWPlayerTurnAction::Init()
{
    // TODO : init action points of align dynamically

	DisplayUI();

	SetupInput();

    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Player Turn"));

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMCTag_BattleCommand);

    UMWInputUtility::BindInputAction(MWGameplayTags::IATag_BC_Move,         ETriggerEvent::Triggered, this, &ThisClass::OnMove);
    UMWInputUtility::BindInputAction(MWGameplayTags::IATag_BC_ChangeLeader, ETriggerEvent::Triggered, this, &ThisClass::OnChangeLeader);
    UMWInputUtility::BindInputAction(MWGameplayTags::IATag_BC_UseItem,      ETriggerEvent::Triggered, this, &ThisClass::OnUseItem);
    UMWInputUtility::BindInputAction(MWGameplayTags::IATag_BC_UseSpirit,    ETriggerEvent::Triggered, this, &ThisClass::OnUseSpirit);
}

void UMWPlayerTurnAction::Uninit()
{
    UMWInputUtility::ClearBindingsForObject(this);

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMCTag_BattleCommand);
}

void UMWPlayerTurnAction::Update(const FMWTurnActionData& InInfo, bool& OutIsFin)
{
	OutIsFin = bActionFinished;

	// action points all used
	// enemy all died
}

void UMWPlayerTurnAction::DisplayUI()
{

}

void UMWPlayerTurnAction::SetupInput()
{
}

void UMWPlayerTurnAction::OnMove(const FInputActionValue& Value)
{
    bActionFinished = true;
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnMove"));
}

void UMWPlayerTurnAction::OnChangeLeader(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnChangeLeader"));
}

void UMWPlayerTurnAction::OnUseItem(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnUseItem"));
}

void UMWPlayerTurnAction::OnUseSpirit(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnUseSpirit"));
}

UE_ENABLE_OPTIMIZATION
