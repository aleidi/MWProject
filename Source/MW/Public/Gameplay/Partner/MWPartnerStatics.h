#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "MWPartnerStatics.generated.h"

UCLASS()
class MW_API UMWPartnerStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Partner", meta = (DefaultToSelf = "Outer"))
	static AActor* GetPartnerActor(AActor* Outer);

	UFUNCTION(BlueprintCallable, Category = "Partner", meta = (DefaultToSelf = "Outer"))
	static void SendGameplayEventToPartner(AActor* Outer, FGameplayTag EventTag, FGameplayEventData Payload);
};