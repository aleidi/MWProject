#pragma once 

#include "GameFramework/Actor.h"
#include "Define/MWStruct.h"
#include "MWBattleUnitPosition.generated.h"

/* AMWBattleUnitPosition
* 
* This class is used to designed where the battle unit should stand.
* In editor, preview mesh will be used to check the result. 
*/
UCLASS(BlueprintType)
class MW_API AMWBattleUnitPosition : public AActor
{
	GENERATED_BODY()

public:
	AMWBattleUnitPosition();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;


#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	TArray<UChildActorComponent*> PreviewActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	bool bShowPreviewMesh;

	/* Check or uncheck UpdatePositionViaPreviewMesh, Position will be updated according to PreviewActors's location.*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true))
	bool UpdatePositionViaPreviewMesh;
#endif

	virtual void OnConstruction(const FTransform& Transform) override;
#endif

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn=true))
	EMWTeamAlign Align = EMWTeamAlign::Player;

	/* The unit number of the alignment attends the battle. One Team can have 3 units at most. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = 1, ClampMax = 3, ExposeOnSpawn=true, NoResetToDefault))
	int32 UnitNo = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn=true))
	int32 Index = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn=true))
	TArray<FVector> Position;
};