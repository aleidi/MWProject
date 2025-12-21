#include "Animation/MWCharacterAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/MWGameplayTags.h"

void UMWCharacterAnimInstance::NativeBeginPlay()
{
	OnMontageStarted.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageStarted);
	OnPlayMontageNotifyBegin.AddDynamic(this, &UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin);

	if (APawn* owningPawn = TryGetPawnOwner())
	{
		if (IAbilitySystemInterface* asi = Cast<IAbilitySystemInterface>(owningPawn))
		{
			if (UAbilitySystemComponent* asc = asi->GetAbilitySystemComponent())
			{
				BattleStateTagHandle = asc->RegisterGameplayTagEvent(
					MWGameplayTags::GP_Character_CombatState,
					EGameplayTagEventType::NewOrRemoved
				).AddUObject(this, &UMWCharacterAnimInstance::OnBattleStateTagChanged);
			}
		}
	}
	
}

void UMWCharacterAnimInstance::ProcessOnMontageStarted(UAnimMontage* MontageInst)
{
}

void UMWCharacterAnimInstance::ProcessOnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
}

void UMWCharacterAnimInstance::OnBattleStateTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	// NewCount > 0 表示 Tag 被添加（进入战斗）
	   // NewCount == 0 表示 Tag 被移除（退出战斗）
	bool bIsInBattleState = (NewCount > 0);

	UE_LOG(LogTemp, Log, TEXT("[%s] Battle state changed: %s"),
		*TryGetPawnOwner()->GetName(),
		bIsInBattleState ? TEXT("Enter Battle") : TEXT("Exit Battle"));

}
