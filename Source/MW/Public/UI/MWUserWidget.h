// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "MWUIInterface.h"
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
	TObjectPtr<UWidgetAnimation> OpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> CloseAnimation;

	TFunction<void()> OnCloseAnimationFinishedCallback = nullptr;


};
