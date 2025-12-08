// Fill out your copyright notice in the Description page of Project Settings.


#include "DSExtends/GameplayMessageSubsystemStatics.h"

float UGameplayMessageSubsystemStatics::GetFloatFromMessage(const FSimpleParamMessage& Message)
{
	// 兼容 double
	if (Message.PayloadType == SimpleParamEnumValue<double>::Value)
	{
		return Message.Get<double>();
	}
	return Message.Get<float>();
}

bool UGameplayMessageSubsystemStatics::GetBoolFromMessage(const FSimpleParamMessage& Message)
{
	return Message.Get<bool>();
}

int32 UGameplayMessageSubsystemStatics::GetIntFromMessage(const FSimpleParamMessage& Message)
{
	return Message.Get<int32>();
}

double UGameplayMessageSubsystemStatics::GetDoubleFromMessage(const FSimpleParamMessage& Message)
{
	// 兼容 float
	if (Message.PayloadType == SimpleParamEnumValue<float>::Value)
	{
		return Message.Get<float>();
	}
	return Message.Get<double>();
}

