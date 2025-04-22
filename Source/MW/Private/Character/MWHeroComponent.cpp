#include "Character/MWHeroComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/MWInputConfig.h"
#include "InputMappingContext.h"
#include "Gameplay/MWGameplayTags.h"
#include "Input/MWInputComponent.h"
#include "Gamemode/MWGamemode.h"
#include "Character/MWPawnData.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
#include "Character/MWPawnExtensionComponent.h"

UMWHeroComponent::UMWHeroComponent(const FObjectInitializer& ObjectInitializer)
{
	// enable tick
	PrimaryComponentTick.bCanEverTick = true;
}

void UMWHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMWHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMWHeroComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}