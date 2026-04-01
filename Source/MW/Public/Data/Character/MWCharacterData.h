#pragma once

#include "Data/MWPrimaryAsset.h"
#include "MWCharacterData.generated.h"

class UMWAbilitySet;

UCLASS()
class UMWCharacterDataBase : public UMWPrimaryAsset
{
	GENERATED_BODY()

public:
	UMWCharacterDataBase();
};

UCLASS()
class UMWCharacterAppearanceData : public UMWCharacterDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMesh> Mesh_Body;
};

UCLASS()
class UMWCharacterAnimData : public UMWCharacterDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Idle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Walk;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Run;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Fall;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Land;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequenceBase> Anim_Hit;
};

USTRUCT(BlueprintType)
struct FMWCharacterDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 Id = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText DisplayName = FText::GetEmpty();

	// ========== Appearance ==========
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance")
	TSoftObjectPtr<UMWCharacterAppearanceData> DefaultAppearance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Appearance")
	TMap<int32, TSoftObjectPtr<UMWCharacterAppearanceData>> ExtraAppearance;

	// ========== Animation ==========
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TSoftClassPtr<UMWCharacterAnimInstance> AnimInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Animation")
	TSoftObjectPtr<UMWCharacterAnimData> DefaultAnimation;

	// ========== Ability ==========
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability")
	TSoftObjectPtr<UMWAbilitySet> DefaultAbilitySet;
};