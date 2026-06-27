#pragma once

#include "Input/MWChargeInputProcessor.h"
#include "Interface/MWManagerInterface.h"
#include "UObject/NoExportTypes.h"
#include "MWSkillInputService.generated.h"

class APawn;

UCLASS()
class MW_API UMWSkillInputService : public UObject, public IMWManagerInterface
{
    GENERATED_BODY()

public:
    virtual void Initialize() override {}
    virtual void Deinitialize() override {}

    UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext = "WorldContext", DisplayName = "GetSkillInputService"))
    static UMWSkillInputService* Get(const UObject* WorldContext);

    bool TryGetChargeRuntimeConfig(APawn* InPawn, const FGameplayTag& InInputTag, FMWChargeRuntimeConfig& OutConfig) const;
    bool RequestCastByInputTag(APawn* InPawn, const FGameplayTag& InInputTag) const;
};