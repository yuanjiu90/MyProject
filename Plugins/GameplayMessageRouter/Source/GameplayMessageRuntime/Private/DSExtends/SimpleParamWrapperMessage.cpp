// Fill out your copyright notice in the Description page of Project Settings.


#include "DSExtends/SimpleParamWrapperMessage.h"

FSimpleParamMessage::FSimpleParamMessage()
{
	PayloadType = 0;
}

FSimpleParamMessage::FSimpleParamMessage(const FProperty* Property, void* Data)
{
	if (CastField<FIntProperty>(Property))
	{
		InitWithValue(*(int32*)Data);
	}
	else if (CastField<FBoolProperty>(Property))
	{
		InitWithValue(*(bool*)Data);
	}
	else if (CastField<FFloatProperty>(Property))
	{
		InitWithValue(*(float*)Data);
	}
	else if (CastField<FDoubleProperty>(Property))
	{
		InitWithValue(*(double*)Data);
	}
	else
	{
		ensureAlwaysMsgf(false,TEXT("不支持的 Property 类型，请检查或自行添加支持的类型"));
	}
}

bool FSimpleParamMessage::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << PayloadType;
	if (PayloadType == 0)
	{
		if (Ar.IsLoading())
		{
			Payload.Empty();
		}
		return true;
	}
	int32 PayloadLen = Payload.Num();
	Ar << PayloadLen;
	if (Ar.IsLoading())
	{
		Payload.SetNumZeroed(PayloadLen);
	}

	Ar.SerializeBits(Payload.GetData(),PayloadLen * 8);

	return true;
}
