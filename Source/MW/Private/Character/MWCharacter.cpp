#include "Character/MWCharacter.h"

#include "Character/MWAppearanceComponent.h"
#include "Character/Movement/MWCharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Controller/MWPlayerController.h"
#include "Data/Character/MWCharacterDataManager.h"
#include "Data/Character/MWCharacterPrimaryData.h"
#include "GameplayAbility/Attribute/MWBattleAttributeSet.h"
#include "GameplayAbility/MWAbilitySet.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Pawn/MWPawnExtensionComponent.h"

AMWCharacter::AMWCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UMWCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationYaw = false;

	AbilitySystemComponent = CreateDefaultSubobject<UMWAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ExtensionComp = CreateDefaultSubobject<UMWPawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	AppearanceComp = CreateDefaultSubobject<UMWAppearanceComponent>(TEXT("AppearanceComponent"));

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetVisibility(false);

	VisualMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VisualMesh"));
	VisualMesh->SetupAttachment(GetMesh());

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


void AMWCharacter::SetupDefaultAbilities()
{
	UMWCharacterDataManager* dataMgr = UMWCharacterDataManager::Get(this);
	if (!dataMgr)
	{
		return;
	}

	const int32 characterId = GetCharacterId();

	// 1) 已加载 -> 立即授予
	if (UMWCharacterPrimaryData* loaded = dataMgr->GetLoadedCharacterPrimaryData(characterId))
	{
		ApplyDefaultAbilitiesFromPrimaryData(loaded);
		return;
	}

	// 2) 未加载 -> 异步加载 "Spawn" bundle，回调里授予
	TWeakObjectPtr<AMWCharacter> weakThis(this);
	dataMgr->AsyncLoadCharacterBundles(
		characterId,
		{ FName(TEXT("Spawn")) },
		FStreamableDelegate::CreateLambda([weakThis, characterId]()
		{
			AMWCharacter* self = weakThis.Get();
			if (!IsValid(self) || !IsValid(self->AbilitySystemComponent))
			{
				return;
			}

			UMWCharacterDataManager* mgr = UMWCharacterDataManager::Get(self);
			if (!mgr)
			{
				return;
			}

			if (UMWCharacterPrimaryData* pd = mgr->GetLoadedCharacterPrimaryData(characterId))
			{
				self->ApplyDefaultAbilitiesFromPrimaryData(pd);
			}
		}));
}

void AMWCharacter::ApplyDefaultAbilitiesFromPrimaryData(UMWCharacterPrimaryData* PrimaryData)
{
	if (!PrimaryData || !AbilitySystemComponent)
	{
		return;
	}

	// Spawn bundle 已加载，DefaultAbilitySet.Get() 应当非空。
	if (UMWAbilitySet* abilitySet = PrimaryData->DefaultAbilitySet.Get())
	{
		AbilityGranetedHandles = MakeShared<FMWAbilitySet_GrantedHandles>();
		abilitySet->GiveToAbilitySystem(AbilitySystemComponent, AbilityGranetedHandles.Get(), this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ApplyDefaultAbilitiesFromPrimaryData: DefaultAbilitySet still not loaded after bundle load."));
	}
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

void AMWCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupDefaultAbilities();
}

void AMWCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UMWCharacterDataManager* dataMgr = UMWCharacterDataManager::Get(this))
	{
		dataMgr->UnloadCharacter(GetCharacterId());
	}

	Super::EndPlay(EndPlayReason);
}

void AMWCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

USkeletalMeshComponent* AMWCharacter::GetDummyMesh() const
{
	return GetMesh();
}

USkeletalMeshComponent* AMWCharacter::GetVisualMesh() const
{
	return VisualMesh;
}
