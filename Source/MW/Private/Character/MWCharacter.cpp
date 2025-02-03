#include "Character/MWCharacter.h"
#include "Character/MWCharacterMovementComponent.h"
#include "Controller/MWPlayerController.h"
#include "Character/MWTargetSelector.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Character/MWHeroComponent.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Character/MWPawnExtensionComponent.h"

AMWCharacter::AMWCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMWCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UMWAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	HeroComp = CreateDefaultSubobject<UMWHeroComponent>(TEXT("HeroComponent"));

	ExtensionComp = CreateDefaultSubobject<UMWPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
}

UMWAbilitySystemComponent* AMWCharacter::GetMWAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AMWCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AMWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (HeroComp)
	{
		HeroComp->InitializePlayerInput(PlayerInputComponent);
	}
}

void AMWCharacter::ChangeBattleState(MWBehaviorState::EBehaviorState NewState)
{
	BehaviorState = NewState;
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
	return false;
}

void AMWCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

UAbilitySystemComponent* AMWCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
