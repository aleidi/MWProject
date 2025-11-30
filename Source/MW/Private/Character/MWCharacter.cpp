#include "Character/MWCharacter.h"
#include "Component/Character/MWCharacterMovementComponent.h"
#include "Controller/MWPlayerController.h"
#include "Components/InputComponent.h"
#include "Component/Pawn/MWPawnExtensionComponent.h"
#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"

AMWCharacter::AMWCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMWCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationYaw = false;

	AbilitySystemComponent = CreateDefaultSubobject<UMWAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExtensionComp = CreateDefaultSubobject<UMWPawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	DummyMesh = GetMesh();
	DummyMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	DummyMesh->SetVisibility(false);

	VisualMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(DummyMesh);

	PrimaryActorTick.bCanEverTick = true;
}

void AMWCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CanUpdatePawnRotation())
	{
		UpdatePawnRotation(DeltaSeconds);
	}
}

void AMWCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	EnableUpdatePawnRotation(true);
}

void AMWCharacter::UnPossessed()
{
	Super::UnPossessed();

	EnableUpdatePawnRotation(false);
}

UAbilitySystemComponent* AMWCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMWAbilitySystemComponent* AMWCharacter::GetMWAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void AMWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMWCharacter::UpdatePawnRotation(float DeltaTime)
{
	const FVector curr_vel = GetVelocity();
	if (curr_vel.SizeSquared2D() > 0.f)
	{
		LastVelocityRotation = curr_vel.Rotation();
	}

	const FRotator desired_rot = FRotator(0.f, LastVelocityRotation.Yaw, 0.f);
	DesiredPawnRotation = FMath::RInterpConstantTo(DesiredPawnRotation, desired_rot, DeltaTime, DesiredRotInterpSpeed);

	const FRotator actor_rot = GetActorRotation();
	SetActorRotation(FMath::RInterpTo(actor_rot, DesiredPawnRotation, DeltaTime, PawnRotInterpSpeed));
}

void AMWCharacter::EnableUpdatePawnRotation(bool bEnabled)
{
	bUpdatePawnRotation = bEnabled;
}

bool AMWCharacter::CanUpdatePawnRotation() const
{
	return bUpdatePawnRotation && BehaviorState == ECharacterBehaviorState::Normal;
}

void AMWCharacter::ChangeBehaviorState(ECharacterBehaviorState NewState)
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

FVector AMWCharacter::GetFloorLocation() const
{
	FVector loc = GetActorLocation() - FVector::UpVector * GetDefaultHalfHeight();

	return loc;
}

void AMWCharacter::SetCharacterLocation(FVector FloorLocation)
{
	const FVector loc = FloorLocation + FVector::UpVector * GetDefaultHalfHeight();
	SetActorLocation(loc);
}

void AMWCharacter::SetOwnerEntity(UMWCharacterEntity* InOwnerEntity)
{
	OwnerEntity = InOwnerEntity;
}

UMWCharacterEntity* AMWCharacter::GetOwnerEntity() const
{
	return OwnerEntity;
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

USkeletalMeshComponent* AMWCharacter::GetDummyMesh() const
{
	return DummyMesh;
}

USkeletalMeshComponent* AMWCharacter::GetVisualMesh() const
{
	return VisualMesh;
}
