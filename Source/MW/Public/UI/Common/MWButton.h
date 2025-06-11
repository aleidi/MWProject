#pragma once

// Include Header
#include "CommonButtonBase.h"
#include "MWButton.generated.h"

// Forward Declare
class UCommonTextBlock;
class USpacer;
class UHorizontalBox;

// Macro

/*
 * @class UMWButton
 * 
 * @brief Implement playing focus/hover/selected animation.
 *
 * @note
 */
UCLASS()
class UMWButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> Txt_BtnText;

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

	/* Button style updated in this function. So it's a good place to set text style.*/
	virtual void SynchronizeProperties() override;

	/** Allows derived classes to take action when the current text style has changed */
	void NativeOnCurrentTextStyleChanged() override;

	void OnInputMethodChanged(ECommonInputType CurrentInputType) override;
};