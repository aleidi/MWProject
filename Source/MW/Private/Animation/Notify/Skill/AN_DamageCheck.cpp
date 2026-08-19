#include "Animation/Notify/Skill/AN_DamageCheck.h"

#include "AbilitySystemGlobals.h"
#include "GameplayAbility/MWAbilitySystemComponent.h"
#include "MWLogChannels.h"

void UAN_DamageCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	AActor* ownerActor = MeshComp->GetOwner();

	// 所有ActorからAbility System Componentを取得
	UAbilitySystemComponent* asc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ownerActor);
	if (!asc)
	{
		UE_LOG(LogMWAnimNotify, Warning, TEXT("[AN_DamageCheck] Owner actor does not have AbilitySystemComponent"));
		return;
	}

	// アクティブなMWChargeableSkillインスタンスを検索
	TArray<FGameplayAbilitySpec>& activatableAbilities = asc->GetActivatableAbilities();
	for (FGameplayAbilitySpec& spec : activatableAbilities)
	{
		if (spec.IsActive())
		{
			// MWChargeableSkillへのキャストを試行
			//UMWChargeableSkill* chargeableSkill = Cast<UMWChargeableSkill>(spec.GetPrimaryInstance());
			//if (chargeableSkill)
			//{
			//	// 設定済みパラメータでダメージ計算を実行
			//	chargeableSkill->OnDamageNotify(DamageEventTag, DamageMultiplier, HitIndex);
			//	
			//	// 対象スキルを実行済みのため探索を終了
			//	return;
			//}
		}
	}

	// アクティブなチャージスキルが見つからない場合は警告を出力
	UE_LOG(LogMWAnimNotify, Warning, TEXT("[AN_DamageCheck] No active MWChargeableSkill found on actor: %s"), *ownerActor->GetName());
}

FString UAN_DamageCheck::GetNotifyName_Implementation() const
{
	if (DamageEventTag.IsValid())
	{
		return FString::Printf(TEXT("Damage Check [%s] x%.2f (Hit %d)"), 
			*DamageEventTag.ToString(), 
			DamageMultiplier, 
			HitIndex);
	}
	
	return FString::Printf(TEXT("Damage Check x%.2f (Hit %d)"), DamageMultiplier, HitIndex);
}