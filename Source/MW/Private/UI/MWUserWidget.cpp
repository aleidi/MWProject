#include "UI/MWUserWidget.h"

#include "Animation/WidgetAnimation.h"
#include "MWLogChannels.h"
#include "View/MVVMView.h"
#include "View/MVVMViewClass.h"

void UMWUserWidget::PlayOpenAnim()
{
	if (IsValid(WA_OpenAnimation))
	{
		PlayAnimationForward(WA_OpenAnimation);
	}
}

void UMWUserWidget::PlayCloseAnim(TFunction<void()> FinishedCallback)
{
	if (IsValid(WA_CloseAnimation))
	{
		PlayAnimationForward(WA_CloseAnimation);

		if (FinishedCallback)
		{
			OnCloseAnimationFinishedCallback = MoveTemp(FinishedCallback);
		}
	}
	else
	{
		if (FinishedCallback)
		{
			FinishedCallback();
		}
	}
}

void UMWUserWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation == WA_CloseAnimation)
	{
		if (OnCloseAnimationFinishedCallback)
		{
			OnCloseAnimationFinishedCallback();

			OnCloseAnimationFinishedCallback = nullptr;
		}
	}
}

UMVVMViewModelBase* UMWUserWidget::SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce)
{
	if (ViewModelName.IsNone())
	{
		UE_LOG(LogMWUI, Warning, TEXT("SetViewModel: ViewModelName is empty on widget %s"), *GetName());

		return nullptr;
	}

	if (!IsValid(ViewModel))
	{
		UE_LOG(LogMWUI, Warning, TEXT("SetViewModel: ViewModel instance is invalid for ViewModelName %s on widget %s"), *ViewModelName.ToString(), *GetName());

		return nullptr;
	}

	UMVVMView* mvvmView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
	if (!mvvmView)
	{
		UE_LOG(LogMWUI, Warning, TEXT("SetViewModel: MVVMView extension not found on widget %s"), *GetName());

		return nullptr;
	}

	if (!bForce && mvvmView->GetViewModel(ViewModelName) != nullptr)
	{
		UE_LOG(LogMWUI, Warning, TEXT("SetViewModel: ViewModel with name %s already exists on widget %s"), *ViewModelName.ToString(), *GetName());

		return nullptr;
	}

	if (!mvvmView->SetViewModel(ViewModelName, ViewModel))
	{
		return nullptr;
	}

	return ViewModel;
}

UMVVMViewModelBase* UMWUserWidget::CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce)
{
	if (!IsValid(ViewModelClass.Get()))
	{
		UE_LOG(LogMWUI, Warning, TEXT("CreateAndSetViewModel: ViewModelClass is invalid for ViewModelName %s on widget %s"), *ViewModelName.ToString(), *GetName());

		return nullptr;
	}

	UMVVMViewModelBase* newViewModel = NewObject<UMVVMViewModelBase>(this, ViewModelClass);
	if (!newViewModel)
	{
		UE_LOG(LogMWUI, Warning, TEXT("CreateAndSetViewModel: Failed to create ViewModel %s on widget %s"), *ViewModelClass->GetName(), *GetName());

		return nullptr;
	}

	return SetViewModel(ViewModelName, newViewModel, bForce);
}

UMVVMViewModelBase* UMWUserWidget::K2_SetViewModel(FName ViewModelName, UMVVMViewModelBase* ViewModel, bool bForce)
{
	return SetViewModel(ViewModelName, ViewModel, bForce);
}

UMVVMViewModelBase* UMWUserWidget::K2_CreateAndSetViewModel(FName ViewModelName, TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce)
{
	return CreateAndSetViewModel(ViewModelName, ViewModelClass, bForce);
}

UMVVMViewModelBase* UMWUserWidget::CreateAndSetViewModelByClass(UClass* ViewModelClass, bool bForce)
{
	if (!IsValid(ViewModelClass) || !ViewModelClass->IsChildOf(UMVVMViewModelBase::StaticClass()))
	{
		UE_LOG(LogMWUI, Warning, TEXT("SetViewModelByClass: Invalid ViewModelClass on widget %s"), *GetName());

		return nullptr;
	}

	const FName viewModelName = GetManualViewModelNameByClass(ViewModelClass);
	if (viewModelName.IsNone())
	{
		return nullptr;
	}

	return CreateAndSetViewModel(viewModelName, ViewModelClass, bForce);
}

UMVVMViewModelBase* UMWUserWidget::K2_CreateAndSetViewModelByClass(TSubclassOf<UMVVMViewModelBase> ViewModelClass, bool bForce)
{
	return CreateAndSetViewModelByClass(ViewModelClass, bForce);
}

FName UMWUserWidget::GetManualViewModelNameByClass(UClass* ViewModelClass) const
{
	if (!IsValid(ViewModelClass) || !ViewModelClass->IsChildOf(UMVVMViewModelBase::StaticClass()))
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetManualViewModelNameByClass: Invalid ViewModelClass on widget %s"), *GetName());

		return NAME_None;
	}

	const UMVVMView* mvvmView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
	if (!mvvmView)
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetManualViewModelNameByClass: MVVMView extension not found on widget %s"), *GetName());

		return NAME_None;
	}

	const UMVVMViewClass* viewClass = mvvmView->GetViewClass();
	if (!viewClass)
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetManualViewModelNameByClass: ViewClass is null on widget %s"), *GetName());

		return NAME_None;
	}

	FName matchedViewModelName = NAME_None;
	int32 matchedSlotCount = 0;

	for (const FMVVMViewClass_Source& source : viewClass->GetSources())
	{
		if (!source.IsViewModel())
		{
			continue;
		}

		UClass* sourceClass = source.GetSourceClass();
		if (sourceClass && ViewModelClass->IsChildOf(sourceClass))
		{
			matchedViewModelName = source.GetName();
			++matchedSlotCount;
		}
	}

	if (matchedSlotCount == 0)
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetManualViewModelNameByClass: No matching ViewModel slot for class %s on widget %s"), *ViewModelClass->GetName(), *GetName());

		return NAME_None;
	}

	if (matchedSlotCount > 1)
	{
		ensureAlwaysMsgf(false, TEXT("GetManualViewModelNameByClass: Multiple ViewModel slots match class %s on widget %s"), *ViewModelClass->GetName(), *GetName());

		return NAME_None;
	}

	return matchedViewModelName;
}

UMVVMViewModelBase* UMWUserWidget::GetViewModel(FName ViewModelName) const
{
	if (ViewModelName.IsNone())
	{
		return nullptr;
	}

	const UMVVMView* mvvmView = Cast<UMVVMView>(GetExtension(UMVVMView::StaticClass()));
	if (!mvvmView)
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetViewModel: MVVMView extension not found on widget %s"), *GetName());

		return nullptr;
	}

	const auto viewModelInterface = mvvmView->GetViewModel(ViewModelName);
	UObject* viewModelObject = viewModelInterface.GetObject();
	if (!viewModelObject)
	{
		return nullptr;
	}

	UMVVMViewModelBase* viewModelBase = Cast<UMVVMViewModelBase>(viewModelObject);
	if (!viewModelBase)
	{
		UE_LOG(LogMWUI, Warning, TEXT("GetViewModel: ViewModel %s on widget %s is not UMVVMViewModelBase"), *ViewModelName.ToString(), *GetName());

		return nullptr;
	}

	return viewModelBase;
}

UMVVMViewModelBase* UMWUserWidget::GetViewModelByClass(UClass* ViewModelClass) const
{
	if (!IsValid(ViewModelClass) || !ViewModelClass->IsChildOf(UMVVMViewModelBase::StaticClass()))
	{
		return nullptr;
	}

	const FName viewModelName = GetManualViewModelNameByClass(ViewModelClass);
	if (viewModelName.IsNone())
	{
		return nullptr;
	}

	return GetViewModel(viewModelName);
}
