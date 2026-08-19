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
	 * 既存のViewModel InstanceをこのWidgetのMVVMView拡張へBindingします。
	 *
	 * @param ViewModelName ViewModelの登録名。MVVMのBinding名と一致する必要があります。
	 * @param ViewModel Bindingする既存のViewModel Instance。
	 * @param bForce trueの場合は同名のViewModelを置換し、falseの場合は未設定時のみ設定します。
	 * @return 成功時はBindingしたViewModel Instance、失敗時はnullptr。
	 */
	UMVVMViewModelBase* SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce = false);

	/**
	 * ViewModel Instanceを生成し、このWidgetのMVVMView拡張へBindingします。
	 *
	 * @param ViewModelName ViewModelの登録名。MVVMのBinding名と一致する必要があります。
	 * @param ViewModelClass 生成するViewModelクラス。
	 * @param bForce trueの場合は同名のViewModelを置換し、falseの場合は未設定時のみ設定します。
	 * @return 成功時は生成したViewModel Instance、失敗時はnullptr。
	 */
	UMVVMViewModelBase* CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	template<typename T> requires std::derived_from<T, UMVVMViewModelBase>
	T* CreateAndSetViewModel(FName ViewModelName, bool bForce = false)
	{
		return Cast<T>(CreateAndSetViewModel(ViewModelName, T::StaticClass(), bForce));
	}

	/**
	 * Blueprintから呼び出せるSetViewModelのWrapperです。
	 */
	UFUNCTION(BlueprintCallable, Category = "MW|MVVM", meta = (DisplayName = "Set Manual ViewModel"))
	UMVVMViewModelBase* K2_SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce = false);

	/**
	 * Blueprintから呼び出せるCreateAndSetViewModelのWrapperです。
	 */
	UFUNCTION(BlueprintCallable, Category = "MW|MVVM", meta = (DisplayName = "Create And Set Manual ViewModel"))
	UMVVMViewModelBase* K2_CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce = false);

	/**
	 * Widget内のMVVM Slotとクラスを照合し、ViewModel Instanceを生成してBindingします。
	 * 失敗時または一致候補が一意でない場合はnullptrを返します。
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
