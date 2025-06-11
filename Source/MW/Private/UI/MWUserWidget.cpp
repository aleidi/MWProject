// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MWUserWidget.h"
#include "Animation/WidgetAnimation.h"

void UMWUserWidget::PlayOpenAnim()
{
	if (IsValid(OpenAnimation))
	{
		PlayAnimationForward(OpenAnimation);
	}
}

void UMWUserWidget::PlayCloseAnim(TFunction<void()> FinishedCallback)
{
	if (IsValid(CloseAnimation))
	{
		PlayAnimationForward(CloseAnimation);

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
	if (Animation == CloseAnimation)
	{
		if (OnCloseAnimationFinishedCallback)
		{
			OnCloseAnimationFinishedCallback();

			OnCloseAnimationFinishedCallback = nullptr;
		}
	}
}
