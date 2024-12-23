#pragma once

#include "MWGameplayTypes.generated.h"

#define DT_CHARACTERINFO TEXT("/Game/Datatable/Character/DT_CharacterInfo.DT_CharacterInfo")

USTRUCT(BlueprintType)
struct FMWFoundActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name = TEXT("NULL");

	UPROPERTY(BlueprintReadWrite)
	bool bLeft;

	UPROPERTY(BlueprintReadWrite)
	float Angle;

	bool operator==(const FMWFoundActorInfo& Rhs)
	{
		return Name == Rhs.Name;
	}

	void Reset();

	FORCEINLINE bool IsValid() const { return Name != TEXT("NULL"); }
};

FORCEINLINE bool operator==(const FMWFoundActorInfo& Lhs, const FMWFoundActorInfo& Rhs)
{
	return Lhs.Name == Rhs.Name;
}

USTRUCT(BlueprintType)
struct FPartyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> CharacterNames;

	FORCEINLINE bool IsValid() const { return CharacterNames.Num() > 0; }
	FORCEINLINE uint32 Num() const { return CharacterNames.Num(); }
};

//================================================//
//================== Data Table ==================//
//================================================//
USTRUCT(BlueprintType)
struct FMWCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Head;
};