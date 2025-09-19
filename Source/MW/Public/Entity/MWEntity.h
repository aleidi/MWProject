#pragma once

// Include Header
#include "UObject/NoExportTypes.h"
#include "Entity/MWObjctId.h"
#include "MWEntity.generated.h"

// Forward Declare
class AActor;

// Define

/*
 * @class UMWEntity
 * 
 * @brief The Entity is like a wrapper of the AActor.
 *		  It has UID, consists of the components that are not derived from UActorComponent.
 *		  All entities are managed by the UMWEntityManager.
 * @note
 */
UCLASS()
class UMWEntity : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize();

	virtual void Uninitialize();

	//===========================================
	// Actor
	//===========================================
public:
	virtual UClass* GetActorClass() const;

	virtual void OnActorLoaded(AActor* InLoadedActor);

	virtual void UnloadActor();

	TObjectPtr<AActor> LoadActor(bool bDeferConstruction = false);

	/* Set actor from extern, so don't create actor internally. */
	void SetLoadedActor(AActor* InLoadedActor);

	FORCEINLINE AActor* GetActor() { return OwnActor.Get(); }

	FORCEINLINE const AActor* GetActor() const { return OwnActor.Get(); }

	/* The overall interface for setting spawn data. 
	*  If you want to set single data, use SetId, SetLocation, SetRotation instead.
	*/
	void SetSpawnData(const FObjectId& Id, const FVector& InLoc, const FRotator& InRot);

protected:
	TWeakObjectPtr<AActor>	OwnActor;

	//===========================================
	// Transform
	//===========================================
public:
	void SetLocation(const FVector& InLoc);

	virtual FVector GetLocation() const;

	virtual FVector GetFloorLocation() const;

	void SetRotation(const FRotator& InRot);

	virtual FRotator GetRotation() const;

	virtual void OnLocationUpdated();

	virtual void OnRotationUpdated();

protected:
	FVector Location;

	FRotator Rotation;

	//===========================================
	// ID
	//===========================================
public:
	FORCEINLINE void SetId(FObjectId NewId) { Uid = NewId; }

	FORCEINLINE FObjectId GetId() const { return Uid; }

protected:
	/* UID is used to specific a entity. */
	FObjectId Uid;
};