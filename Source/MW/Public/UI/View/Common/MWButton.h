#pragma once

// ヘッダーをインクルード
#include "CommonButtonBase.h"
#include "MWButton.generated.h"

// 前方宣言
class UCommonTextBlock;
class USpacer;
class UHorizontalBox;

// 定義

/*
 * @class UMWButton
 * 
 * @brief Focus／Hover／Selectedアニメーションを再生します。
 *
 * @note
 */
UCLASS()
class UMWButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> CTB_BtnText;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Getter = GetButtonText, Setter = SetButtonText, Category = "Content", meta = (DisplayName = "Text"))
	FText ButtonText;

	UFUNCTION(BlueprintCallable, Category = "Button")
	void SetButtonText(FText NewText);

	UFUNCTION(Blueprintpure	, Category = "Button")
	FText GetButtonText() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputMethodChanged, ECommonInputType);
	FOnInputMethodChanged OnInputMethodChangedDele;

protected:
	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	/* ButtonStyleの更新時に呼ばれるため、TextStyleの設定に適しています。 */
	virtual void SynchronizeProperties() override;

	/** 現在のTextStyle変更時に派生クラス固有の処理を行います。 */
	void NativeOnCurrentTextStyleChanged() override;

	void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
};