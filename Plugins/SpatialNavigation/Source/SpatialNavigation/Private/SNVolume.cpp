#include "SNVolume.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "ProceduralMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// 定義
#define GRID_MATERIAL_PATH TEXT("Material'/Navigation3D/M_Grid.M_Grid'")

// デフォルト値を設定
ASNVolume::ASNVolume()
{
	// アクターのTickを毎フレーム呼び出す。不要な場合は無効化することでパフォーマンスを改善できる
	PrimaryActorTick.bCanEverTick = true;

	// プロシージャルメッシュコンポーネントを作成
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->CastShadow = false;
	ProceduralMesh->SetEnableGravity(false);
	ProceduralMesh->bApplyImpulseOnDamage = false;
	ProceduralMesh->SetGenerateOverlapEvents(false);
	ProceduralMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ProceduralMesh->SetCollisionProfileName("NoCollision");
	ProceduralMesh->bHiddenInGame = false;

	// ゲーム実行中はデフォルトでボリュームを非表示にする
	SetActorHiddenInGame(true);

	// グリッドマテリアルを検索して保持
	static ConstructorHelpers::FObjectFinder<UMaterial> materialFinder(GRID_MATERIAL_PATH);
	GridMaterial = materialFinder.Object;
}

void ASNVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 頂点と三角形を格納する配列を作成
	TArray<FVector> vertices;
	TArray<int32> triangles;

	// グリッドのローカル境界（ボリューム形式の中央原点空間）
	const FVector gridMin = GetGridMinLocal();
	const FVector gridMax(gridMin.X + GetGridSizeX(), gridMin.Y + GetGridSizeY(), gridMin.Z + GetGridSizeZ());

	// 線分の始点と終点を定義
	FVector start = FVector::ZeroVector;
	FVector end = FVector::ZeroVector;

	// X方向のグリッド線形状を作成（Y軸に平行）
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

	// Y方向のグリッド線形状を作成（X軸に平行）
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

	// Z方向のグリッド線形状を作成（Z軸に平行）
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

	// CreateMeshSectionへの引き渡しに必要な未使用変数
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FColor> colors;
	TArray<FProcMeshTangent> tangents;

	// 描画する形状をプロシージャルメッシュに追加
	ProceduralMesh->CreateMeshSection(0, vertices, triangles, normals, uvs, colors, tangents, false);

	// 色と不透明度を設定できるようプロシージャルメッシュにマテリアルを設定
	if (GridMaterial != nullptr)
	{
		UMaterialInstanceDynamic* dynamicMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
		dynamicMaterialInstance->SetVectorParameterValue("Color", Color);
		dynamicMaterialInstance->SetScalarParameterValue("Opacity", Color.A);
		ProceduralMesh->SetMaterial(0, dynamicMaterialInstance);
	}
}

// ゲーム開始時またはスポーン時に呼び出される
void ASNVolume::BeginPlay()
{
	Super::BeginPlay();

	// 経路探索用ノードを再確保
	Nodes.Reset();
	Nodes.SetNum(GetTotalDivisions());

	for (SNNode& node : Nodes)
	{
		node.FScore = FLT_MAX;
		node.Neighbors.Reset();
	}

	// 隣接ノード追加用ラムダ
	auto addNeighborIfValid = [&](SNNode* node, const FIntVector& neighbor_coordinates)
	{
		// 隣接座標が有効であることを確認
		if (AreCoordinatesValid(neighbor_coordinates))
		{
			int32 sharedAxes = 0;
			if (node->Coordinates.X == neighbor_coordinates.X)
				++sharedAxes;
			if (node->Coordinates.Y == neighbor_coordinates.Y)
				++sharedAxes;
			if (node->Coordinates.Z == neighbor_coordinates.Z)
				++sharedAxes;

			// 共有軸数が最小値以上で、かつ同一ノードでない場合のみ追加
			if (sharedAxes >= MinSharedNeighborAxes && sharedAxes < 3)
			{
				node->Neighbors.Add(GetNode(neighbor_coordinates));
			}
		}
	};

	// 各ノードの隣接ノードを検索し、座標を設定
	for (int32 z = 0; z < DivisionsZ; ++z)
	{
		for (int32 y = 0; y < DivisionsY; ++y)
		{
			for (int32 x = 0; x < DivisionsX; ++x)
			{
				SNNode* node = GetNode(FIntVector(x, y, z));
				node->Coordinates = FIntVector(x, y, z);

				// 上段の隣接ノード
				{
					// 前列
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 1));
					}
					// 中列
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 1));
					}
					// 後列
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 1));
					}
				}

				// 中段の隣接ノード
				{
					// 前列
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 0));
					}
					// 中列
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 0));
					}
					// 後列
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 0));
					}
				}

				// 下段の隣接ノード
				{
					// 前列
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z - 1));
					}
					// 中列
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z - 1));
					}
					// 後列
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

// 毎フレーム呼び出される
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
	// 出力経路をクリア
	OutPath.Empty();

	struct FOpenSetEntry
	{
		SNNode* Node = nullptr;
		float FScore = FLT_MAX;
	};

	// 元のNodeCompareの順序に合わせ、FScoreが最小の要素を優先
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
			// 経路を再構築
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

	// 位置をグリッド空間へ変換
	const FVector gridSpaceLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), Location);
	const FVector gridMin = GetGridMinLocal();

	// グリッド空間の位置を座標（x、y、z）へ変換
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

	// 座標をグリッド空間の位置へ変換
	const FVector gridMin = GetGridMinLocal();
	FVector gridSpaceLocation = FVector::ZeroVector;
	gridSpaceLocation.X = gridMin.X + (clampedCoordinates.X * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Y = gridMin.Y + (clampedCoordinates.Y * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Z = gridMin.Z + (clampedCoordinates.Z * DivisionSize) + (DivisionSize * 0.5f);

	// グリッド空間の位置をワールド空間へ変換
	return UKismetMathLibrary::TransformLocation(GetActorTransform(), gridSpaceLocation);
}

void ASNVolume::CreateLine(const FVector& Start, const FVector& End, const FVector& Normal, TArray<FVector>& Vertices, TArray<int32>& Triangles)
{
	// 線幅の半分と幅方向を計算
	const float halfLineThickness = LineThickness * 0.5f;
	FVector line = End - Start;
	line.Normalize();

	// 0--------------------------1
	// |           線            |
	// 2--------------------------3
	auto createFlatLine = [&](const FVector& thicknessDirection)
	{
		// 上側の三角形
		Triangles.Add(Vertices.Num() + 2);
		Triangles.Add(Vertices.Num() + 1);
		Triangles.Add(Vertices.Num() + 0);

		// 下側の三角形
		Triangles.Add(Vertices.Num() + 2);
		Triangles.Add(Vertices.Num() + 3);
		Triangles.Add(Vertices.Num() + 1);

		// 頂点0
		Vertices.Add(Start + (thicknessDirection * halfLineThickness));
		// 頂点1
		Vertices.Add(End + (thicknessDirection * halfLineThickness));
		// 頂点2
		Vertices.Add(Start - (thicknessDirection * halfLineThickness));
		// 頂点3
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
