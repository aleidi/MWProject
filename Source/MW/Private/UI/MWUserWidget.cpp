// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MWUserWidget.h"
#include "Animation/WidgetAnimation.h"

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
