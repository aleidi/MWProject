// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MW.h"
#include "CommonActivatableWidget.h"
#include "MWUIInterface.h"
#include "MWActivatableWidget.generated.h"

/**
 * 変更や破棄を伴わず、ライフサイクル中に有効化／無効化できるWidgetの基底クラスです。
 *
 * 主に次の用途で使用します：
 *	- Hierarchyから削除せずに表示状態を切り替えるため、Construct／Destructでは不十分な場合
 *	- Breadcrumbの戻る操作やModalを閉じる操作など、Widgetから戻る処理を共通化する場合
 *	- 有効化可能なWidget Tree上で入力経路を定義するNodeとして扱う場合
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
	TObjectPtr<UWidgetAnimation> WA_OpenAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> WA_CloseAnimation;

	TFunction<void()> OnCloseAnimationFinishedCallback = nullptr;


};
