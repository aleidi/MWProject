// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "MWUIInterface.h"
#include "MWActivatableWidget.generated.h"

/**
 * The base for widgets that are capable of being "activated" and "deactivated" during their lifetime without being otherwise modified or destroyed.
 *
 * This is generally desired for one or more of the following purposes:
 *	- This widget can turn on/off without being removed from the hierarchy (or otherwise reconstructing the underlying SWidgets), so Construct/Destruct are insufficient
 *	- You'd like to be able to "go back" from this widget, whether that means back a breadcrumb, closing a modal, or something else. This is built-in here.
 *	- This widget's place in the hierarchy is such that it defines a meaningful node-point in the tree of activatable widgets through which input is routed to all widgets.
 */
UCLASS()
class UMWActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void PlayOpenAnim();

	void PlayCloseAnim(TFunction<void()> FinishedCallback = nullptr);

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

protected:
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> OpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> CloseAnimation;

	TFunction<void()> OnCloseAnimationFinishedCallback = nullptr;


};
