#pragma once

#include "MW.h"
#include "CommonUserWidget.h"
#include "MWUIInterface.h"
#include "MVVMViewModelBase.h"
#include "MWUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UMWUserWidget : public UCommonUserWidget, public IMWUIInterface
{
	GENERATED_BODY()
	
public:
	void PlayOpenAnim() override;

	void PlayCloseAnim(TFunction<void()> FinishedCallback = nullptr) override;

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> WA_OpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> WA_CloseAnimation;

	TFunction<void()> OnCloseAnimationFinishedCallback = nullptr;

public:
	/**
	 * Binds an existing ViewModel instance to this widget's MVVMView extension.
	 *
	 * @param ViewModelName Name used to register the ViewModel (must match MVVM binding name).
	 * @param ViewModel Existing ViewModel instance to bind.
	 * @param bForce If true, replaces an existing ViewModel with the same name; if false, sets only when missing.
	 * @return The bound ViewModel instance when successful, otherwise nullptr.
	 */
	UMVVMViewModelBase* SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce = false);

	/**
	 * Creates and binds a ViewModel instance to this widget's MVVMView extension.
	 *
	 * @param ViewModelName Name used to register the ViewModel (must match MVVM binding name).
	 * @param ViewModelClass ViewModel class type to instantiate.
	 * @param bForce If true, replaces an existing ViewModel with the same name; if false, sets only when missing.
	 * @return The created ViewModel instance when successful, otherwise nullptr.
	 */
	UMVVMViewModelBase* CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	template<typename T> requires std::derived_from<T, UMVVMViewModelBase>
	T* CreateAndSetViewModel(FName ViewModelName, bool bForce = false)
	{
		return Cast<T>(CreateAndSetViewModel(ViewModelName, T::StaticClass(), bForce));
	}

	/**
	 * Blueprint-callable wrapper for SetViewModel.
	 */
	UFUNCTION(BlueprintCallable, Category = "MW|MVVM", meta = (DisplayName = "Set Manual ViewModel"))
	UMVVMViewModelBase* K2_SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce = false);

	/**
	 * Blueprint-callable wrapper for CreateAndSetViewModel.
	 */
	UFUNCTION(BlueprintCallable, Category = "MW|MVVM", meta = (DisplayName = "Create And Set Manual ViewModel"))
	UMVVMViewModelBase* K2_CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	/**
	 * Creates and binds a ViewModel instance by class match against MVVM slots in the widget.
	 * Returns nullptr on failure or when match is not unique.
	 */
	UMVVMViewModelBase* CreateAndSetViewModelByClass(UClass* ViewModelClass, bool bForce = false);

	template<typename T> requires std::derived_from<T, UMVVMViewModelBase>
	T* CreateAndSetViewModelByClass(bool bForce = false)
	{
		return Cast<T>(CreateAndSetViewModelByClass(T::StaticClass(), bForce));
	}

	UFUNCTION(BlueprintCallable, Category = "MW|MVVM", meta = (DisplayName = "Create And Set Manual ViewModel By Class"))
	UMVVMViewModelBase* K2_CreateAndSetViewModelByClass(TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	FName GetManualViewModelNameByClass(UClass* ViewModelClass) const;

	UMVVMViewModelBase* GetViewModel(FName ViewModelName) const;

	UMVVMViewModelBase* GetViewModelByClass(UClass* ViewModelClass) const;

	template<typename T> requires std::derived_from<T, UMVVMViewModelBase>
	T* GetViewModelByClass() const
	{
		return Cast<T>(GetViewModelByClass(T::StaticClass()));
	}
};
