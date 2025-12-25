// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"
#include "Engine/StreamableManager.h"
#include "CommonUIExtensions.generated.h"

enum class ECommonInputType : uint8;
template <typename T> class TSubclassOf;

class APlayerController;
class UCommonActivatableWidget;
class ULocalPlayer;
class UObject;
class UUserWidget;
struct FFrame;
struct FGameplayTag;
enum class EAsyncWidgetLayerState : uint8;

UCLASS()
class COMMONGAME_API UCommonUIExtensions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UCommonUIExtensions() { }
	
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static ECommonInputType GetOwningPlayerInputType(const UUserWidget* WidgetContextObject);
	
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingTouch(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Global UI Extensions", meta = (WorldContext = "WidgetContextObject"))
	static bool IsOwningPlayerUsingGamepad(const UUserWidget* WidgetContextObject);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static UCommonActivatableWidget* PushContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UCommonActivatableWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void PushStreamedContentToLayer_ForPlayer(const ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, UPARAM(meta = (AllowAbstract = false)) TSoftClassPtr<UCommonActivatableWidget> WidgetClass);

	// 提供异步回调
	static TSharedPtr<FStreamableHandle> PushStreamedContentToLayer_WithStateFunc(const ULocalPlayer* LocalPlayer, const FGameplayTag& LayerName, const TSoftClassPtr<UCommonActivatableWidget>& WidgetClass, const TFunction<void(EAsyncWidgetLayerState, UCommonActivatableWidget*)> StateFunc);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void PopContentFromLayer(UCommonActivatableWidget* ActivatableWidget);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static ULocalPlayer* GetLocalPlayerFromController(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static FName SuspendInputForPlayer(APlayerController* PlayerController, FName SuspendReason);

	static FName SuspendInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendReason);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void ResumeInputForPlayer(APlayerController* PlayerController, FName SuspendToken);

	static void ResumeInputForPlayer(ULocalPlayer* LocalPlayer, FName SuspendToken);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static void SetLayerVisibility(ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerName, ESlateVisibility InVisibility);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Global UI Extensions|Input")
	static FKey GetFirstKeyForInputType(ULocalPlayer* LocalPlayer, const UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Global UI Extensions")
	static void RemoveAllWidgetFromLayer(ULocalPlayer* LocalPlayer, const FGameplayTag GameplayTag);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static bool LayerHasContent(ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) const FGameplayTag LayerName);

	// 包含LayerName
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static bool AboveLayerHasContent(ULocalPlayer* LocalPlayer, UPARAM(meta = (Categories = "UI.Layer")) const FGameplayTag LayerName);
	
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Global UI Extensions")
	static UPrimaryGameLayout* GetRootLayout(ULocalPlayer* LocalPlayer);

private:
	static int32 InputSuspensions;
};
