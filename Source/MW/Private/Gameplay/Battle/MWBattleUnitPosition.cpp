#include "Gameplay/Battle/MWBattleUnitPosition.h"
#include "Engine/StaticMeshActor.h"

#define MAX_PREVIEWACTOR_NO 3

AMWBattleUnitPosition::AMWBattleUnitPosition()
{
	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = sceneComp;

#if WITH_EDITORONLY_DATA
    for (int i = 0; i < MAX_PREVIEWACTOR_NO; ++i)
    {
        UChildActorComponent* childActorComp = CreateDefaultSubobject<UChildActorComponent>(*FString::Printf(TEXT("Preview%d"), i));
        childActorComp->bIsEditorOnly = true;
        childActorComp->SetupAttachment(RootComponent);
        childActorComp->SetHiddenInGame(true);
		PreviewActors.Emplace(childActorComp);
    }
#endif

    Position.SetNum(UnitNo);
}

#if WITH_EDITOR
void AMWBattleUnitPosition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

    auto SetChildActorVisibility = [](UChildActorComponent* ChildActor, bool bShow)
    {
        if (!ChildActor->GetChildActor())
        {
               return;
        }

        auto* mesh = ChildActor->GetChildActor()->GetComponentByClass<UStaticMeshComponent>();
        if (mesh)
        {
            mesh->SetVisibility(bShow);
        }
    };

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AMWBattleUnitPosition, Position))
    {
        if (Position.Num() > MAX_PREVIEWACTOR_NO)
        {
            Position.SetNum(3);
        }

        for (int i = 0; i < Position.Num(); ++i)
        {
            PreviewActors[i]->SetRelativeLocation(Position[i]);
        }
    }

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AMWBattleUnitPosition, UnitNo))
    {
        if (Position.Num() != UnitNo)
        {
            Position.SetNum(UnitNo);
        }

        for (int i = 0; i < MAX_PREVIEWACTOR_NO; ++i)
        {
            if (i < UnitNo)
            {
                SetChildActorVisibility(PreviewActors[i], bShowPreviewMesh);
            }
            else
            {
                SetChildActorVisibility(PreviewActors[i], false);
            }
        }
    }

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AMWBattleUnitPosition, bShowPreviewMesh))
    {
        for (int i = 0; i < MAX_PREVIEWACTOR_NO; ++i)
        {
            if (i < UnitNo)
            {
                SetChildActorVisibility(PreviewActors[i], bShowPreviewMesh);
            }
            else
            {
                SetChildActorVisibility(PreviewActors[i], false);
            }
        }
    }

    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(AMWBattleUnitPosition, UpdatePositionViaPreviewMesh))
    {
		for (int i = 0; i < UnitNo; ++i)
		{
			Position[i] = PreviewActors[i]->GetRelativeLocation();
		}
    }
}

void AMWBattleUnitPosition::OnConstruction(const FTransform& Transform)
{
    for (int i = 0; i < MAX_PREVIEWACTOR_NO; ++i)
    {  
        if (PreviewActors[i]->GetChildActorTemplate())
        {
            auto* mesh = PreviewActors[i]->GetChildActorTemplate()->GetComponentByClass<UStaticMeshComponent>();
            if (mesh)
            {
                if (i < UnitNo)
                {
                    mesh->SetVisibility(bShowPreviewMesh);
                }
                else
                {
                    mesh->SetVisibility(false);
                }
            }
        }
    }

    Position.SetNum(UnitNo);
}

#endif