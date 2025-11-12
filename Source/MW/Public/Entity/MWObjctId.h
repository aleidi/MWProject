#pragma once

// Include
#include "CoreMinimal.h"
#include <atomic>
#include "MWObjctId.generated.h"

// Forward Declare

// Define

/*
 * @class FObjectId
 * 
 * @brief
 *
 * @note
 */
USTRUCT()
struct FObjectId
{
	GENERATED_BODY()

public:
	FObjectId() = default;

	explicit FObjectId(uint64 InId) : RawId(InId) {}

	FObjectId(const FObjectId&) = default;

	FObjectId(FObjectId&&) = default;

	bool operator==(const FObjectId& Other) const { return RawId == Other.RawId; }

	bool operator!=(const FObjectId& Other) const { return !(*this == Other); }

	FObjectId& operator=(const FObjectId&) = default;

	FObjectId& operator=(FObjectId&&) = default;

	friend uint32 GetTypeHash(const FObjectId& InId)
	{
		return GetTypeHash(InId.RawId);
	}

	uint64 GetRaw() const{ return RawId; }

	bool IsValid() const { return RawId != 0; }

private:
	UPROPERTY()
	uint64 RawId = 0;

};