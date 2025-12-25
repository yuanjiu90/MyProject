// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Blueprint/UserWidget.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/CancellableAsyncAction.h"

#include "AsyncAction_LoadWidgetClassAsync.generated.h"

/**
 * Load the widget class asynchronously, the instance the widget after the loading completes, and return it on OnComplete.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoadWidgetClassAsyncDelegate, TSubclassOf<UUserWidget>, UserWidget);

UCLASS(BlueprintType)
class COMMONGAME_API UAsyncAction_LoadWidgetClassAsync : public UCancellableAsyncAction
{
	GENERATED_UCLASS_BODY()

public:
	virtual void Cancel() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, meta=(WorldContext = "WorldContextObject", BlueprintInternalUseOnly="true"))
	static UAsyncAction_LoadWidgetClassAsync* LoadWidgetClassAsync(UObject* WorldContextObject, TSoftClassPtr<UUserWidget> UserWidgetSoftClass, APlayerController* OwningPlayer, bool bSuspendInputUntilComplete = true);

	virtual void Activate() override;

public:

	UPROPERTY(BlueprintAssignable)
	FLoadWidgetClassAsyncDelegate OnComplete;

private:
	
	void OnWidgetLoaded();

	FName SuspendInputToken;
	TWeakObjectPtr<APlayerController> OwningPlayer;
	bool bSuspendInputUntilComplete;
	TSoftClassPtr<UUserWidget> UserWidgetSoftClass;
	TSharedPtr<FStreamableHandle> StreamingHandle;
};
