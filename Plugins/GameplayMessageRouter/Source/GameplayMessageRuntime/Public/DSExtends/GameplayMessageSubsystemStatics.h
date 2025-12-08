// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleParamWrapperMessage.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayMessageSubsystemStatics.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYMESSAGERUNTIME_API UGameplayMessageSubsystemStatics : public UGameplayStatics
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "MessageSubsystem")
	static float GetFloatFromMessage(const FSimpleParamMessage& Message);

	UFUNCTION(BlueprintPure, Category = "MessageSubsystem")
	static bool GetBoolFromMessage(const FSimpleParamMessage& Message);

	UFUNCTION(BlueprintPure, Category = "MessageSubsystem")
	static int32 GetIntFromMessage(const FSimpleParamMessage& Message);

	UFUNCTION(BlueprintPure, Category = "MessageSubsystem")
	static double GetDoubleFromMessage(const FSimpleParamMessage& Message);
};
