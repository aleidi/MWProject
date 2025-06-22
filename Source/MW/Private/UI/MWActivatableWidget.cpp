// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MWActivatableWidget.h"
#include "Animation/WidgetAnimation.h"

void UMWActivatableWidget::PlayOpenAnim()
{
	if (IsValid(WA_OpenAnimation))
	{
		auto* player = PlayAnimationForward(WA_OpenAnimation);
	}
}

void UMWActivatableWidget::PlayCloseAnim(TFunction<void()> FinishedCallback)
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

void UMWActivatableWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
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
