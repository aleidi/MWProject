#pragma once

#include "Pawn/MWPawnComponent.h"
#include "Gameplay/Partner/MWPartnerTypes.h"
#include "MWPartnerComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Partner), meta = (BlueprintSpawnableComponent))
class MW_API UMWPartnerComponent : public UMWPawnComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Partner")
	void SetPartnerRole(EMWPartnerRole InPartnerRole);

	UFUNCTION(BlueprintPure, Category = "Partner")
	EMWPartnerRole GetPartnerRole() const;

	UFUNCTION(BlueprintCallable, Category = "Partner")
	void SetPartnerActor(AActor* InPartnerActor);

	UFUNCTION(BlueprintPure, Category = "Partner")
	AActor* GetPartnerActor() const;

private:
	UPROPERTY()
	EMWPartnerRole PartnerRole = EMWPartnerRole::NotSet;

	UPROPERTY()
	TWeakObjectPtr<AActor> PartnerActor = nullptr;
};