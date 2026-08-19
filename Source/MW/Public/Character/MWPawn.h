#pragma once

#include "AbilitySystemInterface.h"
#include "Interface/MWSelectableInterface.h"
#include "ModularPawn.h"
#include "MWPawn.generated.h"

UCLASS()
class MW_API AMWPawn : public AModularPawn, public IAbilitySystemInterface, public IMWSelectableInterface
{
	GENERATED_BODY()

public:
	AMWPawn(const FObjectInitializer& ObjectInitializer);
	
#pragma region GAS
protected:
	// ASCへの参照。PlayerStateが存在する場合はPlayerState、存在しない場合は本クラスが保持します。
	UPROPERTY()
	class UAbilitySystemComponent* AbilitySystemComponent;

	// AttributeSetBaseへの参照。PlayerStateが存在する場合はPlayerState、存在しない場合は本クラスが保持します。
	//UPROPERTY()
	//class UXXAttributeSetBase* AttributeSetBase;

public:
	inline virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UFUNCTION(Blueprintpure, Category = "MWPawn|GAS", meta = (DisplayName = "GetAbilitySystemComponent"))
	UAbilitySystemComponent* K2_GetAbilitySystemComponent() const;
	void InitStartupAbilities();

	// サーバーでのみ、AcknowledgePossessionより前に呼び出されます。
	virtual void PossessedBy(AController* NewController) override;

protected:
	//UPROPERTY(EditAnywhere, Category = "NWCharacter|GAS")
	//TObjectPtr<class UXXGameplayAbilitySet> StartupAbillities;
#pragma endregion 
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
