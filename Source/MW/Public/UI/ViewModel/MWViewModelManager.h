#pragma once

// Include Header
#include "MW.h"
#include "Interface/MWManagerInterface.h"
#include "MWViewModelManager.generated.h"

// Forward Declare
class UMVVMViewModelBase;
class UMVVMViewModelCollectionObject;

// Define

/*
 * @class UMWViewModelManager
 * 
 * @brief 
 *
 * @note
 */
UCLASS()
class UMWViewModelManager : public UObject, public IMWManagerInterface
{
	GENERATED_BODY()
	
public:
    virtual void Initialize() override;
    virtual void Deinitialize() override {}

    UFUNCTION(BlueprintPure, Category = "Manager", meta = (WorldContext="WorldContext", DisplayName = "GetVMManager"))
	static UMWViewModelManager* Get(const UObject* WorldContext);

	UMVVMViewModelBase* RegisterGlobalViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	template<typename T>
	T* RegisterGlobalViewModel(FName ViewModelName, bool bForce = false)
	{
		return Cast<T>(RegisterGlobalViewModel(ViewModelName, T::StaticClass(), bForce));
	}

	UMVVMViewModelBase* FindGlobalViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass) const;

	template<typename T>
	T* FindGlobalViewModel(FName ViewModelName) const
	{
		return Cast<T>(FindGlobalViewModel(ViewModelName, T::StaticClass()));
	}

private:
	void RegisterDefaultGlobalViewModels();

private:
	UPROPERTY(Transient)
	TObjectPtr<UMVVMViewModelCollectionObject> GlobalViewModelCollection = nullptr;
};
