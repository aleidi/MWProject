#include "Input/MWSkillInputService.h"

#include "Character/MWSkillComponent.h"
#include "System/MWGameInstanceSubsystem.h"

UMWSkillInputService* UMWSkillInputService::Get(const UObject* WorldContext)
{
    if (UMWGameInstanceSubsystem* subsystem = UMWGameInstanceSubsystem::Get(WorldContext))
    {
        return subsystem->GetSkillInputService();
    }

    return nullptr;
}

bool UMWSkillInputService::TryGetChargeRuntimeConfig(APawn* InPawn, const FGameplayTag& InInputTag, FMWChargeRuntimeConfig& OutConfig) const
{
    const UMWSkillComponent* skillComponent = InPawn ? InPawn->FindComponentByClass<UMWSkillComponent>() : nullptr;
    if (!skillComponent)
    {
        return false;
    }

    float maxChargeValue = OutConfig.MaxChargeValue;
    float chargeRate = OutConfig.ChargeRate;
    float dischargeRate = OutConfig.DischargeRate;
    float chargeStartDelay = OutConfig.ChargeStartDelay;

    if (!skillComponent->TryGetChargeParamsByInputTag(InInputTag, maxChargeValue, chargeRate, dischargeRate, chargeStartDelay))
    {
        return false;
    }

    OutConfig.MaxChargeValue = maxChargeValue;
    OutConfig.ChargeRate = chargeRate;
    OutConfig.DischargeRate = dischargeRate;
    OutConfig.ChargeStartDelay = chargeStartDelay;

    return true;
}

bool UMWSkillInputService::RequestCastByInputTag(APawn* InPawn, const FGameplayTag& InInputTag) const
{
    UMWSkillComponent* skillComponent = InPawn ? InPawn->FindComponentByClass<UMWSkillComponent>() : nullptr;
    return skillComponent ? skillComponent->RequestCastByInputTag(InInputTag) : false;
}