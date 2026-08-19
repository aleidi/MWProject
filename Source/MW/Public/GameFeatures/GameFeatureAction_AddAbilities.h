#pragma once

#include "GameFeatureAction_WorldActionBase.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility/MWAbilitySet.h"

#include "GameFeatureAction_AddAbilities.generated.h"

struct FComponentRequestHandle;
struct FWorldContext;
class UInputAction;
class UAttributeSet;
class UDataTable;
class UMWAbilitySet;

USTRUCT(BlueprintType)
struct FMWAbilityGrant
{
	GENERATED_BODY()

	// 付与するAbilityの型
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UGameplayAbility> AbilityType;

	// AbilityにバインドするInputAction（任意）
 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
 	TSoftObjectPtr<UInputAction> InputAction;
};

USTRUCT(BlueprintType)
struct FMWAttributeSetGrant
{
	GENERATED_BODY()

	// 付与するAbilitySet
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UAttributeSet> AttributeSetType;

	// Attributeの初期化に使用するDataTable（任意）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UDataTable> InitializationData;
};

USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	// 追加対象のActor基底クラス
	UPROPERTY(EditAnywhere, Category="Abilities")
	TSoftClassPtr<AActor> ActorClass;

	// 指定クラスのActorへ付与するAbility一覧
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FMWAbilityGrant> GrantedAbilities;

	// 指定クラスのActorへ付与するAttributeSet一覧
	UPROPERTY(EditAnywhere, Category="Attributes")
	TArray<FMWAttributeSetGrant> GrantedAttributes;

	// 指定クラスのActorへ付与するAbilitySet一覧。
	// AbilitySet経由で付与したAbilityはInputActionにバインドされます。
	UPROPERTY(EditAnywhere, Category="Attributes", meta=(AssetBundles="Client,Server"))
	TArray<TSoftObjectPtr<const UMWAbilitySet>> GrantedAbilitySets;
};

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddAbilities

/**
 * 指定型のActorへAbilityとAttributeを付与するGameFeatureActionです。
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Abilities"))
class UGameFeatureAction_AddAbilities final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()

public:
	//~ UGameFeatureActionインターフェース開始
	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ UGameFeatureActionインターフェース終了

	//~ UObjectインターフェース開始
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ UObjectインターフェース終了

	/** 対象Actorクラスへ追加するAbility。 */
	UPROPERTY(EditAnywhere, Category="Abilities", meta=(TitleProperty="ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> AbilitiesList;

private:
	/** Actorへ追加するAbility内容の構造体。 */
	struct FActorExtensions
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes;
		TArray<FMWAbilitySet_GrantedHandles> AbilitySetHandles;
	};

	/* 一時的な拡張情報をキャッシュする構造体。 */
	struct FPerContextData
	{
		TMap<AActor*, FActorExtensions> ActiveExtensions;
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	};
	
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;	

	//~ UGameFeatureAction_WorldActionBaseインターフェース開始
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	//~ UGameFeatureAction_WorldActionBaseインターフェース終了

	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext);
	void AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);
	void RemoveActorAbilities(AActor* Actor, FPerContextData& ActiveData);

	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData)
	{
		//@TODO: 検索のみで追加は不要か？
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), Actor, AbilitiesEntry, ActiveData));
	}
	UActorComponent* FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);
};
