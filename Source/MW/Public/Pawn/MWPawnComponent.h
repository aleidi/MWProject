#pragma once

#include "Components/GameFrameworkComponent.h"
#include "MWPawnComponent.generated.h"

class APawn;
class APlayerController;

UCLASS(Abstract)
class MW_API UMWPawnComponent : public UGameFrameworkComponent
{
    GENERATED_BODY()

public:
	/** このコンポーネントを所有するPawnを取得します。ゲームプレイ中は常に有効ですが、エディタではnullの場合があります。 */
	template <class T>
	T* GetPawn() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawn must be derived from APawn");
		return Cast<T>(GetOwner());
	}

	template <class T>
	T* GetPawnChecked() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "'T' template parameter to GetPawnChecked must be derived from APawn");
		return CastChecked<T>(GetOwner());
	}

	//////////////////////////////////////////////////////////////////////////////
	// Pawnアクセサー。ゲームプレイ中の呼び出しでのみ有効です。
	//////////////////////////////////////////////////////////////////////////////

	/** このコンポーネントを所有するPlayerStateを取得します。複製中のPlayerPawnでは、クライアントでnullの場合があります。 */
	template <class T>
	T* GetPlayerState() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, APlayerState>::Value, "'T' template parameter to GetPlayerState must be derived from APlayerState");
		return GetPawnChecked<APawn>()->GetPlayerState<T>();
	}

	/** このコンポーネントを所有するControllerを取得します。通常、クライアントではnullです。 */
	template <class T>
	T* GetController() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "'T' template parameter to GetController must be derived from AController");
		return GetPawnChecked<APawn>()->GetController<T>();
	}
};