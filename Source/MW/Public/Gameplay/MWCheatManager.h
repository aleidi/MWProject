#pragma once

#include "GameFramework/CheatManager.h"
#include "MWCheatManager.generated.h"

struct FMWActorInfo;

UCLASS()
class MW_API UMWCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(exec,BlueprintCallable,Category="Cheat Manager")
	void TargetSelctedTest(const FMWFoundActorInfo& TargetInfo);
};