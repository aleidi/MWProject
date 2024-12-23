#include "Character/MWCharacter.h"
#include "Character/MWCharacterMovementComponent.h"
#include "Controller/MWPlayerController.h"
#include "Character/MWTargetSelector.h"
#include "Component/MWCameraComponent.h"

AMWCharacter::AMWCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMWCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraComponent = MakeShared<FMWCameraComponent>(this);
}

UAbilitySystemComponent* AMWCharacter::K2_GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMWCharacter::InitStartupAbilities()
{

}

void AMWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!TargetSelector.IsValid())
	{
		TargetSelector = MakeShared<FMWTargetSelector>(NewController);
	}
	else
	{
		TargetSelector->ChangeOwnerController(NewController);
	}
}

void AMWCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AMWCharacter::ChangeBattleState(MWBehaviorState::EBehaviorState NewState)
{
	BehaviorState = NewState;
}

bool AMWCharacter::CastSkill()
{
	/*if (HasLockedTarget() && IsTargetInRange())
	{
		USkeletalMeshComponent* mesh = GetMesh();
		UAnimInstance* animInst = mesh ? mesh->GetAnimInstance() : nullptr;
		
	}*/
	return false;
}

TWeakPtr<FMWTargetSelector> AMWCharacter::GetTargetSelector() const
{
	return TargetSelector->AsWeak();
}

float AMWCharacter::BP_GetNormalizedVelocity() const
{
	return GetNormalizedVelocity();
}

float AMWCharacter::GetNormalizedVelocity() const
{
	if (auto* moveComp = Cast<UMWCharacterMovementComponent>(GetCharacterMovement()))
	{
		return GetVelocity().Size() / moveComp->GetMaxSpeed();
	}

	return 0.f;
}

bool AMWCharacter::GetIsMoving() const
{
	if (auto* moveComp = Cast<UMWCharacterMovementComponent>(GetCharacterMovement()))
	{
		return moveComp->GetRealSpeed() > 0.f;
	}

	return false;
}

bool AMWCharacter::GetCanNormalAtk() const
{
	if (TargetSelector.IsValid())
	{
		TargetSelector->HasLockedTarget();
	}

	return false;
}

void AMWCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TargetSelector.Reset();
}