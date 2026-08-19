#include "Controller/MWPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Character/MWCharacter.h"
#include "Character/MWSkillComponent.h"
#include "Common3DCameraComponent.h"
#include "Common3DCameraModeDataAsset.h"
#include "Pawn/MWPawnExtensionComponent.h"
#include "Data/MWGameplayData.h"
#include "EnhancedInputSubsystems.h"
#include "Gameplay/MWGameplayTags.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Input/MWChargeInputProcessor.h"
#include "Input/MWInputComponent.h"
#include "Input/MWInputUtility.h"
#include "Input/MWSkillInputService.h"
#include "MWGameSingleton.h"
#include "System/MWConsoleVars.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Util/UEDebugUtils.h"


namespace
{
	bool IsSkillInputTag(const FGameplayTag& inputTag)
	{
		return inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot4)
			//|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot1_Charge)
			//|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot2_Charge)
			//|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterSkillSlot3_Charge)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot1)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot2)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_CharacterExtraSkillSlot3)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot1)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot2)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot3)
			|| inputTag.MatchesTagExact(MWGameplayTags::IATag_TPBattle_PartnerSkillSlot4);
	}
}


AMWPlayerController::AMWPlayerController()
{
	MappingOption.bIgnoreAllPressedKeysUntilRelease = true;
}

void AMWPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ChargeInputProcessor)
	{
		ChargeInputProcessor->Tick(DeltaSeconds);
	}

}

AMWCharacter* AMWPlayerController::GetMWCharacter() const
{
	return Cast<AMWCharacter>(GetPawn());
}

void AMWPlayerController::AddInputConfig(const UMWInputConfig* InputConfig, TArray<uint32>& OutBindHandles)
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

void AMWPlayerController::RemoveInputConfig(TArray<uint32>& BindHandles)
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

void AMWPlayerController::K2_AddInputConfig(AActor* Context, const UMWInputConfig* InputConfig)
{
	TArray<uint32> bindHandles;

	AddInputConfig(InputConfig, bindHandles);

	if(bindHandles.Num() > 0)
	{
		TArray<uint32>& cachedBindHandles = BindHandlesCache.FindOrAdd(Context);
		cachedBindHandles.Append(bindHandles);
	}
}

void AMWPlayerController::K2_RemoveInputConfig(AActor* Context)
{
	if (Context)
	{
		if (TArray<uint32>* bindHandles = BindHandlesCache.Find(Context))
		{
			RemoveInputConfig(*bindHandles);
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

	const UMWGameplayData* data = GET_MWSINGLETON()->GetGameplayData();
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
	APawn* pawn = GetPawn<APawn>();
	if (!pawn)
	{
		return;
	}

	UMWSkillInputService* skillInputService = UMWSkillInputService::Get(this);

	// 1) チャージ入力経路
	if (ChargeInputProcessor && ChargeInputProcessor->IsChargeInputTag(InputTag))
	{
		FMWChargeRuntimeConfig runtimeConfig;
		if (skillInputService)
		{
			skillInputService->TryGetChargeRuntimeConfig(pawn, InputTag, runtimeConfig);
		}

		ChargeInputProcessor->HandlePressed(InputTag, runtimeConfig);

		return;
	}

	// 2) スキル入力経路
	if (IsSkillInputTag(InputTag))
	{
		if (skillInputService)
		{
			skillInputService->RequestCastByInputTag(pawn, InputTag);
		}

		return;
	}

	// 3) スキル以外の入力経路
	if (UMWAbilitySystemComponent* mwasc = pawn->FindComponentByClass<UMWAbilitySystemComponent>())
	{
		mwasc->AbilityInputTagPressed(InputTag);
	}
}

void AMWPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	APawn* pawn = GetPawn<APawn>();
	if (!pawn)
	{
		return;
	}

	// 1) チャージ入力解除経路
	if (ChargeInputProcessor && ChargeInputProcessor->IsChargeInputTag(InputTag))
	{
		FGameplayTag castInputTag;
		if (ChargeInputProcessor->HandleReleased(InputTag, castInputTag) && castInputTag.IsValid())
		{
			if (UMWSkillInputService* skillInputService = UMWSkillInputService::Get(this))
			{
				skillInputService->RequestCastByInputTag(pawn, castInputTag);
			}
		}
		return;
	}

	// 2) スキル入力解除は無視（押下時に処理済み）
	if (IsSkillInputTag(InputTag))
	{
		return;
	}

	// 3) スキル以外の入力解除経路
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
	UE_SCREEN_PRINT_CVAR(MWConsoleVars::CVarShowPlayerControllerDebug, 2.f, FColor::Cyan, TEXT("Input_LookStick: %s"), *InputActionValue.ToString());
}

void AMWPlayerController::Input_Crouch(const FInputActionValue& InputActionValue)
{
	UE_SCREEN_PRINT_CVAR(MWConsoleVars::CVarShowPlayerControllerDebug, 2.f, FColor::Cyan, TEXT("Input_Crouch: %s"), *InputActionValue.ToString());
}

void AMWPlayerController::Input_AutoRun(const FInputActionValue& InputActionValue)
{
	UE_SCREEN_PRINT_CVAR(MWConsoleVars::CVarShowPlayerControllerDebug, 2.f, FColor::Cyan, TEXT("Input_AutoRun: %s"), *InputActionValue.ToString());
}

void AMWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ChargeInputProcessor = NewObject<UMWChargeInputProcessor>(this);
}

void AMWPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (ChargeInputProcessor)
	{
		ChargeInputProcessor->Reset();
		ChargeInputProcessor = nullptr;
	}

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
	// サーバーと所有クライアントにカメラを設定
	// Archetype未指定時はEngine.PlayerCameraManagerを生成。通常はArchetypeを指定する
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Camera Managerはマップへ保存しない
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

	// カメラコンポーネントが作成済みか確認
	UC3DCameraComponent* existingC3DCamera = character->FindComponentByClass<UC3DCameraComponent>();
	if (existingC3DCamera)
	{
		return; // カメラコンポーネントは作成済み
	}

	// C3DCameraコンポーネントを作成
	UC3DCameraComponent* c3dCamera = NewObject<UC3DCameraComponent>(character, UC3DCameraComponent::StaticClass(), TEXT("C3DCamera"));
	if (c3dCamera)
	{
		character->AddInstanceComponent(c3dCamera);
		c3dCamera->RegisterComponent();
		c3dCamera->AttachToComponent(character->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

		// Cameraコンポーネントを作成
		UCameraComponent* camera = NewObject<UCameraComponent>(character, UCameraComponent::StaticClass(), TEXT("Camera"));
		if (camera)
		{
			character->AddInstanceComponent(camera);
			camera->RegisterComponent();
			camera->AttachToComponent(c3dCamera, FAttachmentTransformRules::SnapToTargetIncludingScale);
		}

		// デフォルトのカメラモードを設定
		if (const UMWGameplayData* data = GET_MWSINGLETON()->GetGameplayData())
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
