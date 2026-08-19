#pragma once

// ヘッダーをインクルード
#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "SNNode.h"
#include "SNVolume.generated.h"

// 前方宣言
class UMaterial;
class UProceduralMeshComponent;

/*
 * @class ASNVolume
 *
 * @brief
 *
 * @note
 */
UCLASS()
class SPATIALNAVIGATION_API ASNVolume : public AVolume
{
	GENERATED_BODY()

public:
	// アクターのプロパティにデフォルト値を設定
	ASNVolume();

	/**
	* このクラスのインスタンスがエディタに配置またはスポーンされた際に呼び出される。
	* @param	Transform			アクター構築時のトランスフォーム。
	*/
	virtual void OnConstruction(const FTransform& Transform) override;

	// 毎フレーム呼び出される
	virtual void Tick(float DeltaTime) override;

	// 指定座標のノードを取得
	SNNode* GetNode(FIntVector Coordinates);

	const SNNode* GetNode(FIntVector Coordinates) const;

	// 開始位置から目的地までの経路を探索
	UFUNCTION(BlueprintCallable, Category = "SNVolume")
	bool FindPath(const FVector& Start, const FVector& Destination, const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter, TArray<FVector>& OutPath);

	/**
	* ワールド空間の位置をグリッド座標へ変換する。位置がグリッド外の場合は、最も近い座標にクランプする。
	* @param	Location			変換対象の位置
	* @return	変換後の座標
	*/
	UFUNCTION(BlueprintCallable, Category = "SNVolume")
	UPARAM(DisplayName = "Coordinates") FIntVector ConvertLocationToCoordinates(const FVector& Location);

	/**
	* 座標をワールド空間の位置へ変換する。座標がグリッド範囲外の場合は、最も近い座標にクランプする。
	* @param	Coordinates			ワールド空間へ変換する座標
	* @return	変換後のワールド空間位置
	*/
	UFUNCTION(BlueprintCallable, Category = "SNVolume")
	UPARAM(DisplayName = "World Location") FVector ConvertCoordinatesToLocation(const FIntVector& Coordinates);

	// グリッドの総分割数を取得
	UFUNCTION(BlueprintPure, Category = "SNVolume")
	FORCEINLINE int32 GetTotalDivisions() const { return DivisionsX * DivisionsY * DivisionsZ; }

	// グリッドのX軸方向の分割数を取得
	UFUNCTION(BlueprintPure, Category = "SNVolume")
	FORCEINLINE int32 GetDivisionsX() const { return DivisionsX; }

	// グリッドのY軸方向の分割数を取得
	UFUNCTION(BlueprintPure, Category = "SNVolume")
	FORCEINLINE int32 GetDivisionsY() const { return DivisionsY; }

	// グリッドのZ軸方向の分割数を取得
	UFUNCTION(BlueprintPure, Category = "SNVolume")
	FORCEINLINE int32 GetDivisionsZ() const { return DivisionsZ; }

	// グリッドの1区画あたりのサイズを取得
	UFUNCTION(BlueprintPure, Category = "SNVolume")
	FORCEINLINE float GetDivisionSize() const { return DivisionSize; }

protected:
	// ゲーム開始時またはスポーン時に呼び出される
	virtual void BeginPlay() override;

	// アクターがレベルから除去される際に呼び出されるオーバーライド可能な関数
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// グリッドのX軸方向のサイズを取得
	inline float GetGridSizeX() const { return DivisionsX * DivisionSize; }

	// グリッドのY軸方向のサイズを取得
	inline float GetGridSizeY() const { return DivisionsY * DivisionSize; }

	// グリッドのZ軸方向のサイズを取得
	inline float GetGridSizeZ() const { return DivisionsZ * DivisionSize; }

	// グリッドボックスのローカル最小位置を取得
	inline FVector GetGridMinLocal() const
	{
		return FVector(-GetGridSizeX() * 0.5f, -GetGridSizeY() * 0.5f, -GetGridSizeZ() * 0.5f);
	}

private:
	// グリッド線1本分の形状を作成する補助関数
	void CreateLine(const FVector& Start, const FVector& End, const FVector& Normal, TArray<FVector>& Vertices, TArray<int32>& Triangles);

	// 座標の有効性を確認する補助関数
	bool AreCoordinatesValid(const FIntVector& Coordinates) const;

	// 座標をグリッド内の有効範囲にクランプする補助関数
	void ClampCoordinates(FIntVector& Coordinates) const;

private:
	// グリッド描画用プロシージャルメッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SNVolume", meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMesh = nullptr;

	// グリッドのX軸方向の分割数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsX = 10;

	// グリッドのY軸方向の分割数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsY = 10;

	// グリッドのZ軸方向の分割数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsZ = 10;

	// 1区画あたりのサイズ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	float DivisionSize = 100.0f;

	// 隣接ノードとして扱うために必要な共有軸数の最小値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 2))
	int32 MinSharedNeighborAxes = 0;

	// グリッド線の太さ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Aesthetics", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	float LineThickness = 2.0f;

	// グリッドの色
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SNVolume|Aesthetics", meta = (AllowPrivateAccess = "true"))
	FLinearColor Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);

private:
	// プロシージャルメッシュで使用するグリッドマテリアル
	UPROPERTY(Transient)
	TObjectPtr<UMaterial> GridMaterial = nullptr;

	// 経路探索用ノード
	TArray<SNNode> Nodes;
};
