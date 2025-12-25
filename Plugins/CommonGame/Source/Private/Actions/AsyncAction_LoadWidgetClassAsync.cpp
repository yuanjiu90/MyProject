#include "Actions/AsyncAction_LoadWidgetClassAsync.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Engine/Engine.h"
#include "CommonInputSubsystem.h"
#include "CommonUIExtensions.h"

static const FName InputFilterReason_LoadingWidgetClassAsync = FName(TEXT("LoadingWidgetClassAsync"));

UAsyncAction_LoadWidgetClassAsync::UAsyncAction_LoadWidgetClassAsync(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bSuspendInputUntilComplete(true)
{
}

UAsyncAction_LoadWidgetClassAsync* UAsyncAction_LoadWidgetClassAsync::LoadWidgetClassAsync(UObject* InWorldContextObject, TSoftClassPtr<UUserWidget> InUserWidgetSoftClass, APlayerController* InOwningPlayer, bool bSuspendInputUntilComplete)
{
	if (InUserWidgetSoftClass.IsNull())
	{
		FFrame::KismetExecutionMessage(TEXT("LoadWidgetClassAsync was passed a null UserWidgetSoftClass"), ELogVerbosity::Error);
		return nullptr;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(InWorldContextObject, EGetWorldErrorMode::LogAndReturnNull);

	UAsyncAction_LoadWidgetClassAsync* Action = NewObject<UAsyncAction_LoadWidgetClassAsync>();
	Action->UserWidgetSoftClass = InUserWidgetSoftClass;
	Action->OwningPlayer = InOwningPlayer;
	Action->bSuspendInputUntilComplete = bSuspendInputUntilComplete;
	Action->RegisterWithGameInstance(World);

	return Action;
}

void UAsyncAction_LoadWidgetClassAsync::Activate()
{
	SuspendInputToken = bSuspendInputUntilComplete ? UCommonUIExtensions::SuspendInputForPlayer(OwningPlayer.Get(), InputFilterReason_LoadingWidgetClassAsync) : NAME_None;

	TWeakObjectPtr<UAsyncAction_LoadWidgetClassAsync> LocalWeakThis(this);
	StreamingHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		UserWidgetSoftClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &ThisClass::OnWidgetLoaded),
		FStreamableManager::AsyncLoadHighPriority
	);

	// Setup a cancel delegate so that we can resume input if this handler is canceled.
	StreamingHandle->BindCancelDelegate(FStreamableDelegate::CreateWeakLambda(this,
		[this]()
		{
			UCommonUIExtensions::ResumeInputForPlayer(OwningPlayer.Get(), SuspendInputToken);
		})
	);
}

void UAsyncAction_LoadWidgetClassAsync::Cancel()
{
	Super::Cancel();

	if (StreamingHandle.IsValid())
	{
		StreamingHandle->CancelHandle();
		StreamingHandle.Reset();
	}
}

void UAsyncAction_LoadWidgetClassAsync::OnWidgetLoaded()
{
	if (bSuspendInputUntilComplete)
	{
		UCommonUIExtensions::ResumeInputForPlayer(OwningPlayer.Get(), SuspendInputToken);
	}

	// If the load as successful, create it, otherwise don't complete this.
	TSubclassOf<UUserWidget> UserWidgetClass = UserWidgetSoftClass.Get();
	if (UserWidgetClass)
	{
		OnComplete.Broadcast(UserWidgetClass);
	}

	StreamingHandle.Reset();

	SetReadyToDestroy();
}
