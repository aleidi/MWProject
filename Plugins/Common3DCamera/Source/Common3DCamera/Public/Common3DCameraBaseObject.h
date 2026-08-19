#pragma once

#include "CoreMinimal.h"

#include "Common3DCameraBaseObject.generated.h"

class UC3DCameraComponent;

/**
* 本プラグインの全カメラオブジェクトの基底クラス
* すべてのK2（Blueprint）関数はネイティブ関数から呼び出される（例：TickからK2Tick）
*/
UCLASS(classGroup = "C3DCamera", Blueprintable, BlueprintType)
class COMMON3DCAMERA_API UC3DCameraBaseObject : public UObject
{
	GENERATED_BODY()
public:
	//~ UObjectインターフェース開始
#if WITH_ENGINE
	virtual class UWorld* GetWorld() const override;
#endif //WITH_ENGINE
	//~ UObjectインターフェース終了

	virtual void Tick(float DeltaTime);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void K2_Tick(float DeltaTime);

	/** オブジェクトのパラメータを検証 */
	UFUNCTION(BlueprintCallable, Category = "C3D")
	virtual void Validate(bool bWithInterpolation);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Validate"))
	void K2_Validate(bool bWithInterpolation);

	virtual void OnExitCameraMode();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnExitCameraMode"))
	void K2_OnExitCameraMode();
	virtual void OnEnterCameraMode(bool bWithInterpolation);
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEnterCameraMode"))
	void K2_OnEnterCameraMode(bool bWithInterpolation);

	virtual void SwitchToCineCamera();

	void SetCamera(UC3DCameraComponent* Camera);

protected:
	UC3DCameraComponent& GetCamera();
	const UC3DCameraComponent& GetCamera() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	UC3DCameraComponent* GetOwningCamera() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	class AActor* GetOwningActor() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	class APawn* GetOwningPawn() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	class APlayerController* GetPlayerController() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	class APlayerCameraManager* GetPlayerCameraManager() const;

	/** データアセットからデータを初期化する。 */
	virtual void InitPropertyFromDataAsset() {}
private:
	UPROPERTY()
	UC3DCameraComponent* OwningCamera;
};
