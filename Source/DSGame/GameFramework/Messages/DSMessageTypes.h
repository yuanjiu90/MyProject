#pragma once
#include "CoreMinimal.h"

#include "DSMessageTypes.generated.h"

// Bool类型的消息
USTRUCT(BlueprintType)
struct  FDSBoolMessage
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, Category = DSMessage)
	bool bValue;
};

// int32类型的消息
USTRUCT(BlueprintType)
struct  FDSIntMessage
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, Category = DSMessage)
	int32 Value;
};

// AActor类型的消息
USTRUCT(BlueprintType)
struct  FDSActorMessage
{
	GENERATED_BODY();
	UPROPERTY(BlueprintReadWrite, Category = DSMessage)
	TObjectPtr<AActor> Value = nullptr;
};