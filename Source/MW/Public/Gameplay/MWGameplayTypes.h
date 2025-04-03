#pragma once

#include "MWGameplayTypes.generated.h"

#define DT_CHARACTERINFO TEXT("/Game/Datatable/Character/DT_CharacterInfo.DT_CharacterInfo")

/* FMWFoundActorInfo
*  It's a data struct used in searching a selectable actor.
*/
USTRUCT(BlueprintType)
struct FMWFoundActorInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Name = TEXT("");

	UPROPERTY(BlueprintReadWrite)
	bool bLeft = false;

	UPROPERTY(BlueprintReadWrite)
	float Angle = 0.f;

	bool operator==(const FMWFoundActorInfo& Rhs)
	{
		return Name == Rhs.Name;
	}

	void Reset() { Name.Empty(); }

	FORCEINLINE bool IsValid() const { return Name.IsEmpty(); }
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