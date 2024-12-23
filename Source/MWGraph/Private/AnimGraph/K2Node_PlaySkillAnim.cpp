// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimGraph/K2Node_PlaySkillAnim.h"

#include "Containers/UnrealString.h"
#include "EdGraph/EdGraphPin.h"
#include "HAL/Platform.h"
#include "Internationalization/Internationalization.h"
#include "Misc/AssertionMacros.h"
#include "Animation/PlaySkillAnimCallbackProxy.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectPtr.h"

#define LOCTEXT_NAMESPACE "K2Node"

UK2Node_PlaySkillAnim::UK2Node_PlaySkillAnim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(UPlaySkillAnimCallbackProxy, CreateProxyObjectForPlaySkillAnim);
	ProxyFactoryClass = UPlaySkillAnimCallbackProxy::StaticClass();
	ProxyClass = UPlaySkillAnimCallbackProxy::StaticClass();
}

FText UK2Node_PlaySkillAnim::GetTooltipText() const
{
	return LOCTEXT("K2Node_PlaySkillAnim_Tooltip", "Plays a skill anim(montage) on a SkeletalMeshComponent");
}

FText UK2Node_PlaySkillAnim::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("PlaySkillAnim", "Play Skill Anim");
}

FText UK2Node_PlaySkillAnim::GetMenuCategory() const
{
	return LOCTEXT("PlayMontageCategory", "Animation|Montage");
}

void UK2Node_PlaySkillAnim::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	Super::GetPinHoverText(Pin, HoverTextOut);

	static const FName NAME_OnNotifyBegin = FName(TEXT("OnNotifyBegin"));
	static const FName NAME_OnNotifyEnd = FName(TEXT("OnNotifyEnd"));

	if (Pin.PinName == NAME_OnNotifyBegin)
	{
		FText ToolTipText = LOCTEXT("K2Node_PlayMontage_OnNotifyBegin_Tooltip", "Event called when using a PlayMontageNotify or PlayMontageNotifyWindow Notify in a Montage.");
		HoverTextOut = FString::Printf(TEXT("%s\n%s"), *ToolTipText.ToString(), *HoverTextOut);
	}
	else if (Pin.PinName == NAME_OnNotifyEnd)
	{
		FText ToolTipText = LOCTEXT("K2Node_PlayMontage_OnNotifyEnd_Tooltip", "Event called when using a PlayMontageNotifyWindow Notify in a Montage.");
		HoverTextOut = FString::Printf(TEXT("%s\n%s"), *ToolTipText.ToString(), *HoverTextOut);
	}
}

#undef LOCTEXT_NAMESPACE
