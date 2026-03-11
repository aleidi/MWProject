#include "Controller/MWPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Character/MWCharacter.h"
#include "Character/MWTargetSelector.h"
#include "Common3DCameraComponent.h"
#include "Common3DCameraModeDataAsset.h"
#include "Component/Pawn/MWPawnExtensionComponent.h"
#include "Data/MWMasterData.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "Gameplay/MWGameplayTags.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Input/MWInputComponent.h"
#include "Input/MWInputConfig.h"
#include "Input/MWInputUtility.h"
#include "MWGameSingleton.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#define DEBUG_PRINT_FUNC(Time) \
if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Cyan, FString::Printf(TEXT("%s called"), UTF8_TO_TCHAR(__FUNCTION__)));

AMWPlayerController::AMWPlayerController()
{
	MappingOption.bIgnoreAllPressedKeysUntilRelease = true;
}

void AMWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AMWCharacter* AMWPlayerController::GetMWCharacter() const
{
	return Cast<AMWCharacter>(GetPawn());
}

void AMWPlayerController::AddAdditionalInputConfig(const UMWInputConfig* InputConfig, TArray<uint32>& OutBindHandles)
{
	const ULocalPlayer* localPlayer = GetLocalPlayer();
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);
	if (ensureMsgf(mwic, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMWInputComponent or a subclass of it.")))
	{
		mwic->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, OutBindHandles);
	}
}

void AMWPlayerController::RemoveAdditionalInputConfig(TArray<uint32>& BindHandles)
{
	const ULocalPlayer* localPlayer = GetLocalPlayer();
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);
	if (ensureMsgf(mwic, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMWInputComponent or a subclass of it.")))
	{
		mwic->RemoveBinds(BindHandles);
	}
}

void AMWPlayerController::K2_AddAdditionalInputConfig(UObject* Context, const UMWInputConfig* InputConfig)
{
	TArray<uint32> bindHandles;

	AddAdditionalInputConfig(InputConfig, bindHandles);

	if(bindHandles.Num() > 0)
	{
		TArray<uint32>& cachedBindHandles = BindHandlesCache.FindOrAdd(Context);
		cachedBindHandles.Append(bindHandles);
	}
}

void AMWPlayerController::K2_RemoveAdditionalInputConfig(UObject* Context)
{
	if (Context)
	{
		if (TArray<uint32>* bindHandles = BindHandlesCache.Find(Context))
		{
			RemoveAdditionalInputConfig(*bindHandles);
			BindHandlesCache.Remove(Context);
		}
	}
}

void AMWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* localPlayer = Cast<ULocalPlayer>(GetLocalPlayer());
	check(localPlayer);

	UEnhancedInputLocalPlayerSubsystem* subsystem = localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	subsystem->ClearAllMappings();

	const UMWMasterData* data = MWSINGLETON->GetMasterData();
	{
		if (UMWInputConfig* inputConfig = data->FindInputConfig(MWGameplayTags::IMC_TPDefault))
		{
			UMWInputUtility::EnableMappingContext(this, inputConfig, MappingOption);

			UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);
			if (ensureMsgf(mwic, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMWInputComponent or a subclass of it.")))
			{
				mwic->AddInputMappings(inputConfig, subsystem);

				TArray<uint32> bindHandles;
				uint32 tmpBindHandle = 0;

				mwic->BindAbilityActions(inputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ bindHandles);
				mwic->BindNativeAction(inputConfig, MWGameplayTags::IATag_TPDefault_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, tmpBindHandle);
				bindHandles.Add(tmpBindHandle);
				mwic->BindNativeAction(inputConfig, MWGameplayTags::IATag_TPDefault_LookAt, ETriggerEvent::Triggered, this, &ThisClass::Input_LookAt, tmpBindHandle);
				bindHandles.Add(tmpBindHandle);
				mwic->BindNativeAction(inputConfig, MWGameplayTags::IATag_TPDefault_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, tmpBindHandle);
				bindHandles.Add(tmpBindHandle);

				BindHandlesCache.Add(this, bindHandles);
			}
		}
	}
}

void AMWPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const APawn* pawn = GetPawn<APawn>();
	if (UMWAbilitySystemComponent* mwasc = pawn->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagPressed(InputTag);
	}
}

void AMWPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* pawn = GetPawn<APawn>();
	if (UMWAbilitySystemComponent* mwasc = pawn->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagReleased(InputTag);
	}
}

void AMWPlayerController::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* pawn = GetPawn<APawn>();
	AController* controller = pawn ? pawn->GetController() : nullptr;

	if (controller)
	{
		const FVector2D value = InputActionValue.Get<FVector2D>();
		const FRotator rot(0.f, controller->GetControlRotation().Yaw, 0.f);

		if (value.X != 0.f)
		{
			const FVector dir = rot.RotateVector(FVector::RightVector);
			pawn->AddMovementInput(dir, value.X);
		}

		if (value.Y != 0.f)
		{
			const FVector dir = rot.RotateVector(FVector::ForwardVector);
			pawn->AddMovementInput(dir, value.Y);
		}
	}
}

void AMWPlayerController::Input_LookAt(const FInputActionValue& InputActionValue)
{
	const FVector2D value = InputActionValue.Get<FVector2D>();

	if (value.X != 0.f)
	{
		AddYawInput(value.X);
	}

	if (value.Y != 0.f)
	{
		AddPitchInput(-value.Y);
	}
}

void AMWPlayerController::Input_LookStick(const FInputActionValue& InputActionValue)
{
	DEBUG_PRINT_FUNC(2.f);
}

void AMWPlayerController::Input_Crouch(const FInputActionValue& InputActionValue)
{
	DEBUG_PRINT_FUNC(2.f);
}

void AMWPlayerController::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	DEBUG_PRINT_FUNC(2.f);
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

void AMWPlayerController::OnBattleBegin()
{
}

void AMWPlayerController::OnBattleEnd()
{
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

void AMWPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	SetupCameraComponents();
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

void AMWPlayerController::SetupCameraComponents()
{
	ACharacter* character = GetMWCharacter();
	if (!character)
	{
		return;
	}

	// Check if camera components already exist
	UC3DCameraComponent* existingC3DCamera = character->FindComponentByClass<UC3DCameraComponent>();
	if (existingC3DCamera)
	{
		return; // Camera components already exist
	}

	// Create C3DCamera component
	UC3DCameraComponent* c3dCamera = NewObject<UC3DCameraComponent>(character, UC3DCameraComponent::StaticClass(), TEXT("C3DCamera"));
	if (c3dCamera)
	{
		character->AddInstanceComponent(c3dCamera);
		c3dCamera->RegisterComponent();
		c3dCamera->AttachToComponent(character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		// Create Camera component
		UCameraComponent* camera = NewObject<UCameraComponent>(character, UCameraComponent::StaticClass(), TEXT("Camera"));
		if (camera)
		{
			character->AddInstanceComponent(camera);
			camera->RegisterComponent();
			camera->AttachToComponent(c3dCamera, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}

		// Setup default camera modes
		if (const UMWMasterData* data = MWSINGLETON->GetMasterData())
		{
			checkf(data->DefaultCameraModesAssets.Num() > 0, TEXT("No camera mode assets"));

			c3dCamera->ClearAllCameraModes();

			for (auto& asset : data->DefaultCameraModesAssets)
			{
				c3dCamera->AddCameraMode(asset.Get());
			}

			c3dCamera->SetCameraMode(data->DefaultCameraModesAssets[0]->ModeTag, true);			
		}
	}
}
