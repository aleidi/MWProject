#pragma once

// Include Header
#include "UI/MWUserWidget.h"
#include "MWMenuButton.generated.h"

// Forward Declare
class UBorder;
class USpacer;
class UHorizontalBox;
class UMWButton;
class USizeBox;

enum class ECommonInputType : uint8;
// Macro

/*
 * @class UMWMenuButton
 * 
 * @brief Implement playing focus/hover animation. Focus and Hover share the same animation.
 *
 * @note
 */
UCLASS()
class UMWMenuButton : public UMWUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> SB_ButtonSize;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UBorder> BD_BackGround;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USpacer> SP_Indent;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HB_Layout;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UMWButton> WBP_Button;

public:
	/* If true, the Text will be indented according to IndentRate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuButton|Content|Indent")
	bool bUseIndent = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuButton|Content|Indent", meta = (EditCondition = "bUseIndent", EditConditionHides))
	FSlateChildSize IndentNormalSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MenuButton|Content|Indent", meta = (EditCondition = "bUseIndent", EditConditionHides))
	FSlateChildSize IndentHoverSize;

	UPROPERTY(EditAnywhere, Category = "MenuButton|Content")
	FText ButtonText;

	UPROPERTY(EditAnywhere, Category = "MenuButton|Size")
	float WidgetWidth;

	UPROPERTY(EditAnywhere, Category = "MenuButton|Size")
	float WidgetHeight;

	UPROPERTY(EditAnywhere, Category = "MenuButton|BackGround")
	bool bUseImageAsBackGround = false;

	UPROPERTY(EditAnywhere, Category = "MenuButton|BackGround", meta = (DisplayName = "BackGround Image", EditCondition = bUseImageAsBackGround, EditConditionHides))
	TObjectPtr<UMaterialInterface> BGImg;

	UPROPERTY(EditAnywhere, Category = "MenuButton|BackGround", meta = (EditCondition = bUseImageAsBackGround, EditConditionHides))
	FName FlickerVarName;

	UPROPERTY(EditAnywhere, Category = "MenuButton|BackGround", meta = (EditCondition = bUseImageAsBackGround, EditConditionHides))
	float FlickerFrequenc = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, BlueprintSetter = "SetTextHorizontalAlignment", Category = "MenuButton|Content", meta = (DisplayName = "HorizontalAlignment"))
	TEnumAsByte<EHorizontalAlignment> TextHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter, BlueprintSetter = "SetTextVerticalAlignment", Category = "MenuButton|Content", meta = (DisplayName = "VerticalAlignment"))
	TEnumAsByte<EVerticalAlignment> TextVerticalAlignment = EVerticalAlignment::VAlign_Fill;

public:
	UFUNCTION(BlueprintCallable, Category="MenuButton|Content")
	void SetTextHorizontalAlignment(EHorizontalAlignment NewHorizontalAlignment);

	UFUNCTION(BlueprintCallable, Category="MenuButton|Content")
	void SetTextVerticalAlignment(EVerticalAlignment NewVerticalAlignment);

	UMWButton* GetButton() const;

protected:
	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

	void OnHovered();

	void OnUnhovered();

	void OnFocusReceived();

	void OnFocusLost();

private:
	bool IsIndentValid() const;

	bool IsAlignValid() const;

	void SetIndentSize(FSlateChildSize NewSize);

	void SetFocusSetting();

	void EnableBGFlicker();

	void DisableBGFlicker();

	void IndentAndEnableFlicker();

	void UnindentAndDisableFlicker();

	void OnInputMethodChanged(ECommonInputType CurrentInputType);

protected:
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MidBg;
};