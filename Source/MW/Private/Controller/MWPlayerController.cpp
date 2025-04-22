#include "Controller/MWPlayerController.h"
#include "Gameplay/Battle/MWBattleSystem.h"
#include "Character/MWTargetSelector.h"
#include "GameFramework/Character.h"
#include "Character/MWCharacter.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "Character/MWPawnExtensionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "System/MWAssetManager.h"
#include "Data/MWMasterData.h"
#include "Input/MWInputConfig.h"
#include "Input/MWInputComponent.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Gameplay/MWGameplayTags.h"

AMWPlayerController::AMWPlayerController()
{
	
}

void AMWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AMWCharacter* AMWPlayerController::GetMWCharacter() const
{
	return Cast<AMWCharacter>(GetPawn());
}

void AMWPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	const ULocalPlayer* local_player = Cast<ULocalPlayer>(GetLocalPlayer());
	check(local_player);

	UEnhancedInputLocalPlayerSubsystem* subsystem = local_player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	subsystem->ClearAllMappings();

	if (auto* data = UMWAssetManager::Get().GetMasterData())
	{
		if (UMWInputConfig* input_config = data->InputConfig.Get())
		{
			if (UEnhancedInputUserSettings* settings = subsystem->GetUserSettings())
			{
				if (FMWInputMappingContextWithPriority* mapping = input_config->InputMappingContext.Find(IMCTag))
				{
					if (mapping->Mapping.Get())
					{
						settings->RegisterInputMappingContext(mapping->Mapping);

						// Structure to hold one-time initialization
						struct FMappingOption
						{
							FModifyContextOptions Option;
							FMappingOption()
							{
								Option.bIgnoreAllPressedKeysUntilRelease = false;
							}
						};
						static FMappingOption mapping_option;

						subsystem->AddMappingContext(mapping->Mapping, mapping->Priority, mapping_option.Option);
					}
				}
			}

			// The MW Input Component has some additional functions to map Gameplay Tags to an Input Action.
			// If you want this functionality but still want to change your input component class, make it a subclass
			// of the UMWInputComponent or modify this component accordingly.
			UMWInputComponent* mwic = Cast<UMWInputComponent>(InputComponent);
			if (ensureMsgf(mwic, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UMWInputComponent or a subclass of it.")))
			{
				// Add the key mappings that may have been set by the player
				mwic->AddInputMappings(input_config, subsystem);

				// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
				// be triggered directly by these input actions Triggered events. 
				TArray<uint32> bind_handles;
				mwic->BindAbilityActions(input_config, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ bind_handles);

				mwic->BindNativeAction(input_config, MWGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
				mwic->BindNativeAction(input_config, MWGameplayTags::InputTag_LookAt, ETriggerEvent::Triggered, this, &ThisClass::Input_LookAt, /*bLogIfNotFound=*/ false);
				//mwic->BindNativeAction(input_config, MWGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
				//mwic->BindNativeAction(input_config, MWGameplayTags::InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
				mwic->BindNativeAction(input_config, MWGameplayTags::InputTag_AutoRun, ETriggerEvent::Triggered, this, &ThisClass::Input_AutoRun, /*bLogIfNotFound=*/ false);
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
		mwasc->AbilityInputTagPressed(InputTag);
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

}

void AMWPlayerController::Input_Crouch(const FInputActionValue& InputActionValue)
{

}

void AMWPlayerController::Input_AutoRun(const FInputActionValue& InputActionValue)
{

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
