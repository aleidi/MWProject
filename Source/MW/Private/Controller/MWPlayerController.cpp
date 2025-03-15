#include "Controller/MWPlayerController.h"
#include "Subsystem/MWBattleSystem.h"
#include "Character/MWTargetSelector.h"
#include "GameFramework/Character.h"
#include "Character/MWCharacter.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Character/MWPawnExtensionComponent.h"
#include "Camera/MWPlayerCameraManager.h"

AMWPlayerController::AMWPlayerController()
{
	
}

AMWCharacter* AMWPlayerController::GetMWCharacter() const
{
	return Cast<AMWCharacter>(GetPawn());
}

void AMWPlayerController::SwitchToLeftTarget()
{
	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		comp->SwitchToLeftTarget();
	}
}

void AMWPlayerController::SwitchToRightTarget()
{
	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		comp->SwitchToRightTarget();
	}
}

void AMWPlayerController::CancelSelect()
{
	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		comp->CancelSelect();
	}
}

void AMWPlayerController::LockTarget()
{
	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		comp->LockTarget();
	}
}

void AMWPlayerController::UnlockTarget()
{
	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		comp->UnlockTarget();
	}
}

void AMWPlayerController::NotifyPlayerAttackted(FMWFoundActorInfo Attacker, FMWFoundActorInfo Attackee)
{
	if (Attackee.Name == GetPawn()->GetName())
	{
		if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
		{
			comp->ForceLockIfNoTarget(Attacker);
		}
	}
}

void AMWPlayerController::OnOtherCharacterDied(FMWFoundActorInfo InCharacter)
{
	if (InCharacter.IsValid() && InCharacter.Name != GetCharacter()->GetName())
	{
		if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
		{
			comp->OnTargetNotExisted(InCharacter);
		}
	}
}

void AMWPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMWPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if (UMWAbilitySystemComponent* asc = GetMWAbilitySystemComponent())
	{
		asc->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
}

void AMWPlayerController::BindDelegates()
{
	if (UMWBattleSystem* mwbs = GetWorld()->GetSubsystem<UMWBattleSystem>())
	{
		mwbs->OnAttack.AddDynamic(this, &AMWPlayerController::NotifyPlayerAttackted);
		mwbs->OnCharacterDied.AddDynamic(this, &AMWPlayerController::OnOtherCharacterDied);
	}
}

UAbilitySystemComponent* AMWPlayerController::GetAbilitySystemComponent() const
{
	const IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(GetPawn());
	return asi ? asi->GetAbilitySystemComponent() : nullptr;
}

UMWAbilitySystemComponent* AMWPlayerController::GetMWAbilitySystemComponent() const
{
	return Cast<UMWAbilitySystemComponent>(GetAbilitySystemComponent());
}

void AMWPlayerController::SpawnPlayerCameraManager()
{
	// servers and owning clients get cameras
	// If no archetype specified, spawn an Engine.PlayerCameraManager.  NOTE all games should specify an archetype.
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save camera managers into a map
	if (PlayerCameraManagerClass != NULL)
	{
		PlayerCameraManager = GetWorld()->SpawnActor<APlayerCameraManager>(PlayerCameraManagerClass, SpawnInfo);
	}
	else
	{
		PlayerCameraManager = GetWorld()->SpawnActor<APlayerCameraManager>(SpawnInfo);
	}

	if (PlayerCameraManager != NULL)
	{
		PlayerCameraManager->InitializeFor(this);
	}
	else
	{
		UE_LOG(LogPlayerController, Log, TEXT("Couldn't Spawn PlayerCameraManager for Player!!"));
	}
}
