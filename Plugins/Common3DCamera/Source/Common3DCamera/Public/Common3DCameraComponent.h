#pragma once

#include "Common3DTypes.h"
#include "Components/SceneComponent.h"
#include "GameplayTagContainer.h"

#include "Common3DCameraComponent.generated.h"

class UC3DCameraModeDataAsset;

class AC3DCameraVolume;
class UC3DCameraBaseObject;
class UC3DCameraMoveObject;
class UC3DCameraFOVObject;
class UC3DCameraFadingObject;
class UC3DCameraFollowTerrainObject;
class UC3DCameraLockOnTargetObject;
class UC3DCameraShakesObject;
class UC3DCameraModeScript;
class APawn;
class APlayerController;
class APlayerCameraManager;

/** プラグインのメインコンポーネント */
UCLASS(Blueprintable, classGroup = "C3DCamera", meta = (BlueprintSpawnableComponent))
class COMMON3DCAMERA_API UC3DCameraComponent : public USceneComponent
{
	GENERATED_BODY()

	/** カメラモードの変更時に呼び出される（新規設定、オーバーライド設定、オーバーライド解除） */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCameraModeChangedDelegate);

public:
	UC3DCameraComponent();

	//~ USceneComponentインターフェース開始
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
	//~ USceneComponentインターフェース終了

	//~ UActorComponentインターフェース開始
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//~ UActorComponentインターフェース終了

	UFUNCTION(BlueprintCallable, Category = "C3D")
	virtual void ValidateComponents(bool bWithInterpolation);

	virtual void OnBeginOverlapCameraVolume(AC3DCameraVolume* CameraVolume);
	virtual void OnEndOverlapCameraVolume(AC3DCameraVolume* CameraVolume);

	//~ ズーム関数開始
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void ZoomIn();
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void ZoomOut();
	UFUNCTION(BlueprintCallable, Category = "C3D|Zoom")
	void SetCameraDistance(float NewDistance, bool bInterpolate);
	//~ ズーム関数終了

	/** 
	* CameraModesAssetsから@CameraModeTagに対応するカメラモードを設定する
	* @param bForceSet trueの場合、CanSetCameraModeを無視する
	*/
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	virtual void SetCameraMode(FGameplayTag CameraModeTag, bool bWithInterpolation, bool bForceSet = false);

	/** オーバーライド用関数 */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	bool CanSetCameraMode(FGameplayTag CameraModeTag) const;

	/** CameraVolumeを検索し、有効な場合はそのタグ、それ以外は@DefaultCameraModeTagを返す */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	FGameplayTag GetInitialCameraModeTag() const;

	/** @FindOverlapCameraVolumeで重複中のCameraVolumeを検索し、ボリュームとタグが有効な場合はタグ、それ以外は無効なタグを返す */
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	FGameplayTag GetCameraModeTagFromOverlapCameraVolume() const;

	/** オーバーライド用関数。デフォルトでは現在のタグが有効な場合はそのタグ、それ以外は@DefaultCameraModeTagを返す */
	UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = "C3D|CameraMode")
	FGameplayTag GetDesiredCameraModeTag() const;

	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	FGameplayTag GetCurrentCameraModeTag() const;

	const UC3DCameraModeDataAsset* GetCurrentCameraMode() const;
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	UC3DCameraModeDataAsset* GetCurrentCameraMode();

	/** 現在のカメラモードを変更せず、別の変数でオーバーライドする */
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	virtual void SetCustomCameraMode(UC3DCameraModeDataAsset* CameraMode, bool bWithInterpolation);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void ResetCustomCameraMode(bool bWithInterpolation);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	bool IsSetCustomCameraMode() const;

	//~ カメラモード管理関数開始
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void AddCameraMode(UC3DCameraModeDataAsset* CameraMode);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void RemoveCameraMode(FGameplayTag CameraModeName);
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	bool HasCameraMode(FGameplayTag CameraModeName) const;
	UC3DCameraModeDataAsset* GetCameraMode(FGameplayTag CameraModeName) const;
	UFUNCTION(BlueprintPure, Category = "C3D|CameraMode")
	UC3DCameraModeDataAsset* GetCameraMode(FGameplayTag CameraModeName);
	UFUNCTION(BlueprintCallable, Category = "C3D|CameraMode")
	void ClearAllCameraModes();
	//~ カメラモード管理関数終了

	UFUNCTION(BlueprintPure, Category = "C3D")
	virtual AC3DCameraVolume* FindOverlapCameraVolume() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	float GetCameraDistance() const;

	UFUNCTION(BlueprintPure, Category = "C3D")
	FVector GetCameraLocation() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	FRotator GetCameraRotation() const;

	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraMoveObject* GetCameraLocationObject() const { return MoveObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFOVObject* GetCameraFOVObject() const { return FOVObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFadingObject* GetCameraFadingObject() const { return FadingObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraFollowTerrainObject* GetCameraFollowTerrainObject() const { return FollowTerrainObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraShakesObject* GetCameraShakesObject() const { return CameraShakesObject; }
	//UFUNCTION(BlueprintPure, Category = "C3D")
	//UC3DCameraLockOnTargetObject* GetCameraLockOnTargetObject() const { return LockOnTargetObject; }

	UFUNCTION(BlueprintPure, Category = "C3D")
	virtual bool IsCineCameraActive() const;

	/** 初期カメラモードの設定中はtrueを返す（一部のカメラオブジェクトを正しく初期化するために必要） */
	bool IsSettingInitialCameraMode() const;

	template<typename T>
	T* CreateCameraObject(TSubclassOf<T> ObjectClass)
	{
		static_assert(TPointerIsConvertibleFromTo<T, const UC3DCameraBaseObject>::Value, "'T' template parameter to CreateCameraObject must be derived from UC3DCameraBaseObject");
		UClass* objClass = ObjectClass != nullptr ? ObjectClass.Get() : T::StaticClass();
		return CastChecked<T>(CreateCameraObjectImpl(objClass));
	}

	UFUNCTION(BlueprintPure, Category = "C3D")
	APawn* GetOwningPawn() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	APlayerController* GetPlayerController() const;
	UFUNCTION(BlueprintPure, Category = "C3D")
	APlayerCameraManager* GetPlayerCameraManager() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "C3D")
	FOnCameraModeChangedDelegate OnCameraModeChangedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C3D")
	FC3DCameraDebugRules DebugRules;

protected:
	/* CameraModeAssetsからカメラモードを生成 */
	virtual void CreateCameraModesFromAssets();

	virtual void SetInitialCameraMode();

	virtual void InternalSetCameraMode(UC3DCameraModeDataAsset* NewCameraMode, TOptional<FGameplayTag> CameraModeTag, bool bWithInterpolation);

	virtual void NotifyExitCameraMode();
	virtual void NotifyEnterCameraMode(bool bWithInterpolation);

	virtual void CreateAllCameraObjects();

	void CheckCameraModesErros();
	void PutLog(const FString& Message, float MessageTime = 5.f);

	void RegisterConsoleCommands();

protected:
	UC3DCameraBaseObject* CreateCameraObjectImpl(TSubclassOf<UC3DCameraBaseObject> ObjectClass);

protected:
	bool bIsSettingInitialCameraMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C3D|Settings")
	FGameplayTag DefaultCameraModeTag;

	/** カメラモードを保持するデータアセット */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C3D|Settings")
	TArray<UC3DCameraModeDataAsset*> CameraModesAssets;

	/** FGameplayTagで分類したカメラモードインスタンスの格納先 */
	UPROPERTY()
	TMap<FGameplayTag, UC3DCameraModeDataAsset*> SortedCameraModes;

	UPROPERTY()
	UC3DCameraModeDataAsset* CustomCameraMode;

	FGameplayTag CurrentCameraModeTag;

	UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	TSubclassOf<UC3DCameraMoveObject> MoveObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraFOVObject> FOVObjectClass;
	UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	TSubclassOf<UC3DCameraFadingObject> FadingObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraFollowTerrainObject> FollowTerrainObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraShakesObject> CameraShakesObjectClass;
	//UPROPERTY(EditAnywhere, NoClear, Category = "C3D|Settings|CameraObjects")
	//TSubclassOf<UC3DCameraLockOnTargetObject> LockOnTargetObjectClass;

	UPROPERTY(Transient)
	UC3DCameraMoveObject* MoveObject;
	//UPROPERTY(Transient)
	//UC3DCameraFOVObject* FOVObject;
	UPROPERTY(Transient)
	UC3DCameraFadingObject* FadingObject;
	//UPROPERTY(Transient)
	//UC3DCameraFollowTerrainObject* FollowTerrainObject;
	//UPROPERTY(Transient)
	//UC3DCameraShakesObject* CameraShakesObject;
	//UPROPERTY(Transient)
	//UC3DCameraLockOnTargetObject* LockOnTargetObject;

	UPROPERTY(Transient)
	TArray<UC3DCameraBaseObject*> CameraObjList;

	// ビューターゲット切り替え後のシネカメラを処理済みか
	bool bHandledCineCamera = false;

private:
	UPROPERTY()
	UC3DCameraModeDataAsset* EmptyCameraMode;
	/* Blueprintの詳細パネルにFATPCCameraModeを正しく表示するために使用 */
	UPROPERTY()
	FC3DCameraMode CameraModeDEV;
};
