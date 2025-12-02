#pragma once
#include "CoreMinimal.h"
#include "DSEnhancedInputComponentType.generated.h"

//GA按键绑定
UENUM(BlueprintType)
enum class EDSGameplayAbilityInputBinds : uint8
{
	// 开火
	Fire = 0						UMETA(DisplayName = "Fire"),
	// 跳跃
	Jump							UMETA(DisplayName = "Jump"),
	// 换弹
	Reload							UMETA(DisplayName = "Reload"),
};

/**
 * FDSInputAction
 *
 *	Struct used to map a input action to a gameplay input tag.
 */
USTRUCT(BlueprintType)
struct FDSInputAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputName"))
	FName InputName;

	//是否绑定Trigger而不是Start，trigger可能会导致多次触发
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputName"))
	bool bIsTrigger = false;

	//是否检测长按
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (Categories = "InputName"))
	bool bCheckLongPress = false;
};

/**
 * UDSInputConfig
 *
 *	用于绑定InputAction与按键使用
 */
UCLASS(BlueprintType, Const)
class UDSInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UDSInputConfig(const FObjectInitializer& ObjectInitializer){};

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindNativeInputActionForName(const FName InputName) const
	{
		for (const FDSInputAction& Action : NativeInputActions)
		{
			if (Action.InputAction && Action.InputName == InputName)
			{
				return Action.InputAction;
			}
		}
		return nullptr;
	};

	UFUNCTION(BlueprintCallable, Category = "Lyra|Pawn")
	const UInputAction* FindAbilityInputActionForName(const FName InputName) const
	{
		for (const FDSInputAction& Action : AbilityInputActions)
		{
			if (Action.InputAction && Action.InputName == InputName)
			{
				return Action.InputAction;
			}
		}
		return nullptr;
	};

public:
	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and must be manually bound.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FDSInputAction> NativeInputActions;

	// List of input actions used by the owner.  These input actions are mapped to a gameplay tag and are automatically bound to abilities with matching input tags.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FDSInputAction> AbilityInputActions;

	//短按与长按对应的按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (TitleProperty = "InputAction"))
	TMap<FName, FName> LongPressKeyMap;
};
