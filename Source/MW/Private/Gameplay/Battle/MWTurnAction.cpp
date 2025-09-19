#include "Gameplay/Battle/MWTurnAction.h"
#include "Input/MWInputUtility.h"
#include "Kismet/GameplayStatics.h"
#include "Input/MWInputConfig.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
UE_DISABLE_OPTIMIZATION

void UMWTurnAction::SetActionUnits(const TArray<FMWTeam>& InPlayerUnits, const TArray<FMWTeam>& InEnemyUnits)
{
    PlayerUnits = InPlayerUnits;
    EnemyUnits  = InEnemyUnits;
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

}

void UMWPlayerTurnAction::Uninit()
{
    UMWInputUtility::ClearBindingsForObject(this);

    UMWInputUtility::DisableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand);
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
    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCommand);

    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_ChangeLeader, ETriggerEvent::Triggered, this, &ThisClass::OnChangeLeader);
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_UseItem, ETriggerEvent::Triggered, this, &ThisClass::OnUseItem);
    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCommand, MWGameplayTags::IATag_BC_UseSpirit, ETriggerEvent::Triggered, this, &ThisClass::OnUseSpirit);

    UMWInputUtility::BindInputAction(MWGameplayTags::IMC_Basic, MWGameplayTags::IATag_Basic_Direction, ETriggerEvent::Triggered, this, &ThisClass::OnSelectEnemy);
}

void UMWPlayerTurnAction::OnMove(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnMove"));

    UMWInputUtility::DisableAllInputAction(this, MWGameplayTags::IMC_BattleCommand, MWGameplayTags::BattleCommands);

    // Bind character control input
    for(const FGameplayTag& controlTag : MWGameplayTags::BattleCharacterAction)
    {
        UMWInputUtility::BindInputAction(MWGameplayTags::IMC_BattleCharacterAction, controlTag, ETriggerEvent::Triggered, this, &ThisClass::OnCharacterActionControl, controlTag);
	}

    UMWInputUtility::EnableMappingContext(UGameplayStatics::GetPlayerController(this, 0), MWGameplayTags::IMC_BattleCharacterAction);
}

void UMWPlayerTurnAction::OnChangeLeader(const FInputActionValue& Value)
{
    bActionFinished = true;
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

void UMWPlayerTurnAction::OnSelectEnemy(const FInputActionValue& Value)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("OnSelectEnemy"));
}

void UMWPlayerTurnAction::OnCharacterActionControl(FGameplayTag Tag)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("OnCharacterAction : %s"), *Tag.GetTagName().ToString()));
}

UE_ENABLE_OPTIMIZATION