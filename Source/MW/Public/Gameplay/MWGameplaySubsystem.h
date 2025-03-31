#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MWGameplaySubsystem.generated.h"


UCLASS()
class MW_API UMWGameplaySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Gameplay")
	UDataTable* GetTableCharacterInfo() const;
};