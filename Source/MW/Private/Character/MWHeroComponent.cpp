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

void UMWHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UMWHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UMWHeroComponent::RemoveAdditionalInputConfig(const UMWInputConfig* InputConfig)
{

}

void UMWHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* pawn = GetPawn<APawn>();
	if (!pawn)
	{
		return;
	}

	const APlayerController* player_controller = GetController<APlayerController>();
	check(player_controller);

	const ULocalPlayer* local_player = Cast<ULocalPlayer>(player_controller->GetLocalPlayer());
	check(local_player);

	UEnhancedInputLocalPlayerSubsystem* subsystem = local_player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(subsystem);

	subsystem->ClearAllMappings();

	if (auto* data = UMWAssetManager::Get().GetMasterData())
	{
		if (const UMWInputConfig * input_config = data->InputConfig.Get())
		{
			if (UEnhancedInputUserSettings* settings = subsystem->GetUserSettings())
			{
				for (const FMWInputMappingContextWithPriority& mapping : input_config->InputMappingContext)
				{
					if (IsValid(mapping.Mapping))
					{
						settings->RegisterInputMappingContext(mapping.Mapping);

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

						subsystem->AddMappingContext(mapping.Mapping, mapping.Priority, mapping_option.Option);
					}
				}
			}

			// The MW Input Component has some additional functions to map Gameplay Tags to an Input Action.
			// If you want this functionality but still want to change your input component class, make it a subclass
			// of the UMWInputComponent or modify this component accordingly.
			UMWInputComponent* mwic = Cast<UMWInputComponent>(PlayerInputComponent);
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

	if (UMWPawnExtensionComponent* comp = UMWPawnExtensionComponent::FindPawnExtensionComponent(pawn))
	{
		comp->InitializeTargetSelector(player_controller);
	}
}

void UMWHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const APawn* pawn = GetPawn<APawn>();
	if (UMWAbilitySystemComponent* mwasc = pawn->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagPressed(InputTag);
	}
}

void UMWHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* pawn = GetPawn<APawn>();
	if (UMWAbilitySystemComponent* mwasc = pawn->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagPressed(InputTag);
	}
}

void UMWHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
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

void UMWHeroComponent::Input_LookAt(const FInputActionValue& InputActionValue)
{
	APawn* pawn = GetPawn<APawn>();

	if (pawn)
	{
		const FVector2D value = InputActionValue.Get<FVector2D>();

		if (value.X != 0.f)
		{
			pawn->AddControllerYawInput(value.X);
		}

		if (value.Y != 0.f)
		{
			pawn->AddControllerPitchInput(-value.Y);
		}
	}
}

void UMWHeroComponent::Input_AutoRun(const FInputActionValue& InputActionValue)
{

}
