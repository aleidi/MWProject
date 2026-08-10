#include "SNVolume.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Define
#define GRID_MATERIAL_PATH TEXT("Material'/Navigation3D/M_Grid.M_Grid'")

// Sets default values
ASNVolume::ASNVolume()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the procedural mesh component
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->CastShadow = false;
	ProceduralMesh->SetEnableGravity(false);
	ProceduralMesh->bApplyImpulseOnDamage = false;
	ProceduralMesh->SetGenerateOverlapEvents(false);
	ProceduralMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ProceduralMesh->SetCollisionProfileName("NoCollision");
	ProceduralMesh->bHiddenInGame = false;

	// By default, hide the volume while the game is running
	SetActorHiddenInGame(true);

	// Find and save the grid material
	static ConstructorHelpers::FObjectFinder<UMaterial> materialFinder(GRID_MATERIAL_PATH);
	GridMaterial = materialFinder.Object;
}

void ASNVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Create arrays to store the vertices and the triangles
	TArray<FVector> vertices;
	TArray<int32> triangles;

	// Grid local bounds (Volume-style centered space)
	const FVector gridMin = GetGridMinLocal();
	const FVector gridMax(gridMin.X + GetGridSizeX(), gridMin.Y + GetGridSizeY(), gridMin.Z + GetGridSizeZ());

	// Define variables for the start and end of the line
	FVector start = FVector::ZeroVector;
	FVector end = FVector::ZeroVector;

	// Create the X lines geometry (parallel to Y axis)
	for (int32 z = 0; z <= DivisionsZ; ++z)
	{
		start.Z = end.Z = gridMin.Z + (z * DivisionSize);
		for (int32 x = 0; x <= DivisionsX; ++x)
		{
			start.X = end.X = gridMin.X + (x * DivisionSize);
			start.Y = gridMin.Y;
			end.Y = gridMax.Y;

			CreateLine(start, end, FVector::UpVector, vertices, triangles);
		}
	}

	// Create the Y lines geometry (parallel to X axis)
	for (int32 z = 0; z <= DivisionsZ; ++z)
	{
		start.Z = end.Z = gridMin.Z + (z * DivisionSize);
		for (int32 y = 0; y <= DivisionsY; ++y)
		{
			start.Y = end.Y = gridMin.Y + (y * DivisionSize);
			start.X = gridMin.X;
			end.X = gridMax.X;

			CreateLine(start, end, FVector::UpVector, vertices, triangles);
		}
	}

	// Create the Z lines geometry (parallel to Z axis)
	for (int32 x = 0; x <= DivisionsX; ++x)
	{
		start.X = end.X = gridMin.X + (x * DivisionSize);
		for (int32 y = 0; y <= DivisionsY; ++y)
		{
			start.Y = end.Y = gridMin.Y + (y * DivisionSize);
			start.Z = gridMin.Z;
			end.Z = gridMax.Z;

			CreateLine(start, end, FVector::ForwardVector, vertices, triangles);
		}
	}

	// Unused variables that are required to be passed to CreateMeshSection
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FColor> colors;
	TArray<FProcMeshTangent> tangents;

	// Add the geometry to the procedural mesh so it will render
	ProceduralMesh->CreateMeshSection(0, vertices, triangles, normals, uvs, colors, tangents, false);

	// Set the material on the procedural mesh so its color/opacity can be configurable
	if (GridMaterial != nullptr)
	{
		UMaterialInstanceDynamic* dynamicMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
		dynamicMaterialInstance->SetVectorParameterValue("Color", Color);
		dynamicMaterialInstance->SetScalarParameterValue("Opacity", Color.A);
		ProceduralMesh->SetMaterial(0, dynamicMaterialInstance);
	}
}

// Called when the game starts or when spawned
void ASNVolume::BeginPlay()
{
	Super::BeginPlay();

	// Reallocate nodes used for pathfinding
	Nodes.Reset();
	Nodes.SetNum(GetTotalDivisions());

	for (SNNode& node : Nodes)
	{
		node.FScore = FLT_MAX;
		node.Neighbors.Reset();
	}

	// Helper lambda for adding a neighbor
	auto addNeighborIfValid = [&](SNNode* node, const FIntVector& neighbor_coordinates)
	{
		// Make sure the neighboring coordinates are valid
		if (AreCoordinatesValid(neighbor_coordinates))
		{
			int32 sharedAxes = 0;
			if (node->Coordinates.X == neighbor_coordinates.X)
				++sharedAxes;
			if (node->Coordinates.Y == neighbor_coordinates.Y)
				++sharedAxes;
			if (node->Coordinates.Z == neighbor_coordinates.Z)
				++sharedAxes;

			// Only add the neighbor if we share more axes with it than the required min shared neighbor axes and it isn't the same node as us
			if (sharedAxes >= MinSharedNeighborAxes && sharedAxes < 3)
			{
				node->Neighbors.Add(GetNode(neighbor_coordinates));
			}
		}
	};

	// For each node, find its neighbors and assign its coordinates
	for (int32 z = 0; z < DivisionsZ; ++z)
	{
		for (int32 y = 0; y < DivisionsY; ++y)
		{
			for (int32 x = 0; x < DivisionsX; ++x)
			{
				SNNode* node = GetNode(FIntVector(x, y, z));
				node->Coordinates = FIntVector(x, y, z);

				// Above neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 1));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 1));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 1));
					}
				}

				// Middle neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 0));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 0));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 0));
					}
				}

				// Below neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z - 1));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z - 1));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z - 1));
					}
				}
			}
		}
	}
}

void ASNVolume::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Nodes.Reset();

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ASNVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

SNNode* ASNVolume::GetNode(FIntVector Coordinates)
{
	ClampCoordinates(Coordinates);

	const int32 divisionPerLevel = DivisionsX * DivisionsY;
	const int32 index = (Coordinates.Z * divisionPerLevel) + (Coordinates.Y * DivisionsX) + Coordinates.X;
	return &Nodes[index];
}

const SNNode* ASNVolume::GetNode(FIntVector Coordinates) const
{
	ClampCoordinates(Coordinates);

	const int32 divisionPerLevel = DivisionsX * DivisionsY;
	const int32 index = (Coordinates.Z * divisionPerLevel) + (Coordinates.Y * DivisionsX) + Coordinates.X;
	return &Nodes[index];
}

bool ASNVolume::FindPath(const FVector& Start, const FVector& Destination, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter, TArray<FVector>& OutPath)
{
	// Clear the out path
	OutPath.Empty();

	struct FOpenSetEntry
	{
		SNNode* Node = nullptr;
		float FScore = FLT_MAX;
	};

	// Prioritize the lowest FScore, matching the original NodeCompare ordering.
	auto openSetPredicate = [](const FOpenSetEntry& Lhs, const FOpenSetEntry& Rhs)
	{
		return Lhs.FScore < Rhs.FScore;
	};

	TArray<FOpenSetEntry> openSet;
	TSet<SNNode*> closedSet;
	TMap<SNNode*, SNNode*> predMap;
	TMap<SNNode*, float> gScores;

	SNNode* startNode = GetNode(ConvertLocationToCoordinates(Start));
	SNNode* endNode = GetNode(ConvertLocationToCoordinates(Destination));

	auto hScore = [endNode](SNNode* node)
	{
		return FVector::Distance(FVector(endNode->Coordinates), FVector(node->Coordinates));
	};
	auto gScore = [&gScores](SNNode* node)
	{
		if (const float* score = gScores.Find(node))
		{
			return *score;
		}

		return FLT_MAX;
	};
	auto distance = [](SNNode* node1, SNNode* node2)
	{
		return FVector::Distance(FVector(node1->Coordinates), FVector(node2->Coordinates));
	};

	startNode->FScore = hScore(startNode);
	openSet.HeapPush(FOpenSetEntry{ startNode, startNode->FScore }, openSetPredicate);
	gScores.Add(startNode, 0.0f);

	while (openSet.Num() > 0)
	{
		FOpenSetEntry currentEntry;
		openSet.HeapPop(currentEntry, openSetPredicate);

		SNNode* current = currentEntry.Node;
		if (currentEntry.FScore != current->FScore)
		{
			continue;
		}

		if (closedSet.Contains(current))
		{
			continue;
		}

		if (current == endNode)
		{
			// Rebuild the path
			OutPath.Add(ConvertCoordinatesToLocation(current->Coordinates));

			while (true)
			{
				if (SNNode** previousNode = predMap.Find(current))
				{
					current = *previousNode;
					OutPath.Insert(ConvertCoordinatesToLocation(current->Coordinates), 0);
				}
				else
				{
					return true;
				}
			}
		}

		closedSet.Add(current);

		for (SNNode* neighbor : current->Neighbors)
		{
			if (closedSet.Contains(neighbor))
			{
				continue;
			}

			const float tentativeGScore = gScore(current) + distance(current, neighbor);

			if (tentativeGScore < gScore(neighbor))
			{
				TArray<AActor*> outActors;
				const FVector worldLocation = ConvertCoordinatesToLocation(neighbor->Coordinates);
				const bool traversable = !UKismetSystemLibrary::BoxOverlapActors(
					GetWorld(),
					worldLocation,
					FVector(GetDivisionSize() * 0.5f),
					ObjectTypes,
					ActorClassFilter,
					TArray<AActor*>(),
					outActors);

				if (traversable)
				{
					predMap.Add(neighbor, current);
					gScores.Add(neighbor, tentativeGScore);
					const float fScore = tentativeGScore + hScore(neighbor);
					neighbor->FScore = fScore;
					openSet.HeapPush(FOpenSetEntry{ neighbor, fScore }, openSetPredicate);
				}
			}
		}
	}

	return false;
}

FIntVector ASNVolume::ConvertLocationToCoordinates(const FVector& Location)
{
	FIntVector coordinates;

	// Convert the location into grid space
	const FVector gridSpaceLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Location);
	const FVector gridMin = GetGridMinLocal();

	// Convert the grid space location to a coordinate (x,y,z)
	const FVector relativeLocation = gridSpaceLocation - gridMin;
	coordinates.X = FMath::FloorToInt(relativeLocation.X / DivisionSize);
	coordinates.Y = FMath::FloorToInt(relativeLocation.Y / DivisionSize);
	coordinates.Z = FMath::FloorToInt(relativeLocation.Z / DivisionSize);

	ClampCoordinates(coordinates);

	return coordinates;
}

FVector ASNVolume::ConvertCoordinatesToLocation(const FIntVector& Coordinates)
{
	FIntVector clampedCoordinates(Coordinates);
	ClampCoordinates(clampedCoordinates);

	// Convert the coordinates into a grid space location
	const FVector gridMin = GetGridMinLocal();
	FVector gridSpaceLocation = FVector::ZeroVector;
	gridSpaceLocation.X = gridMin.X + (clampedCoordinates.X * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Y = gridMin.Y + (clampedCoordinates.Y * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Z = gridMin.Z + (clampedCoordinates.Z * DivisionSize) + (DivisionSize * 0.5f);

	// Convert the grid space location into world space
	return UKismetMathLibrary::TransformLocation(GetActorTransform(), gridSpaceLocation);
}

void ASNVolume::CreateLine(const FVector& Start, const FVector& End, const FVector& Normal, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	// Calculate the half line thickness and the thickness direction
	const float halfLineThickness = LineThickness * 0.5f;
	FVector line = End - Start;
	line.Normalize();

	// 0--------------------------1
	// |          line           |
	// 2--------------------------3
	auto createFlatLine = [&](const FVector& thicknessDirection)
	{
		// Top triangle
		Triangles.Add(Vertices.Num() + 2);
		Triangles.Add(Vertices.Num() + 1);
		Triangles.Add(Vertices.Num() + 0);

		// Bottom triangle
		Triangles.Add(Vertices.Num() + 2);
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num() + 1);

		// Vertex 0
		Vertices.Add(Start + (thicknessDirection * halfLineThickness));
		// Vertex 1
		Vertices.Add(End + (thicknessDirection * halfLineThickness));
		// Vertex 2
		Vertices.Add(Start - (thicknessDirection * halfLineThickness));
		// Vertex 3
		Vertices.Add(End - (thicknessDirection * halfLineThickness));
	};

	FVector direction1 = UKismetMathLibrary::Cross_VectorVector(line, Normal);
	FVector direction2 = UKismetMathLibrary::Cross_VectorVector(line, direction1);

	createFlatLine(direction1);
	createFlatLine(direction2);
}

bool ASNVolume::AreCoordinatesValid(const FIntVector& Coordinates) const
{
	return Coordinates.X >= 0 && Coordinates.X < DivisionsX &&
		Coordinates.Y >= 0 && Coordinates.Y < DivisionsY &&
		Coordinates.Z >= 0 && Coordinates.Z < DivisionsZ;
}

void ASNVolume::ClampCoordinates(FIntVector& Coordinates) const
{
	Coordinates.X = FMath::Clamp(Coordinates.X, 0, DivisionsX - 1);
	Coordinates.Y = FMath::Clamp(Coordinates.Y, 0, DivisionsY - 1);
	Coordinates.Z = FMath::Clamp(Coordinates.Z, 0, DivisionsZ - 1);
}
