// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleParamWrapperMessage.generated.h"

/**
 * 简单参数 Wrapper ，原生不支持 float , int , bool 这些
 */

USTRUCT(BlueprintType)
struct GAMEPLAYMESSAGERUNTIME_API FSimpleParamMessage
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<uint8> Payload;

	UPROPERTY()
	uint8 PayloadType;
	
	FSimpleParamMessage();

	template <typename DataType>
	FSimpleParamMessage(const DataType& DataValue);

	FSimpleParamMessage(const FProperty* Property, void* Data);

	template <typename DataType>
	DataType Get() const;
	
	bool NetSerialize( FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess );

	template <typename DataType>
	void InitWithValue(const DataType& DataValue);
};

template<typename Type>
struct SimpleParamEnumValue 
{
	static constexpr uint8 Value = MAX_uint8;
	using type = SimpleParamEnumValue;
};

template<typename T>
struct TIsSupportedSimpleParamType
{
	template<typename U>
	static constexpr bool Test()
	{
		return SimpleParamEnumValue<U>::Value != MAX_uint8;	
	}
	static constexpr bool Value = Test<T>();
};

#define SUPPORT_PARAM_TYPE(TYPE,TYPE_ID)\
template<> struct SimpleParamEnumValue<TYPE>\
{\
static constexpr uint8 Value = TYPE_ID;\
};\

SUPPORT_PARAM_TYPE(void, 0);
SUPPORT_PARAM_TYPE(int, 1);
SUPPORT_PARAM_TYPE(bool, 2);
SUPPORT_PARAM_TYPE(char, 3);
SUPPORT_PARAM_TYPE(signed char, 4);
SUPPORT_PARAM_TYPE(unsigned char, 5);
SUPPORT_PARAM_TYPE(wchar_t, 6);
#ifdef __cpp_char8_t
SUPPORT_PARAM_TYPE(char8_t, 7);
#endif
SUPPORT_PARAM_TYPE(char16_t, 8);
SUPPORT_PARAM_TYPE(char32_t, 9);
SUPPORT_PARAM_TYPE(short, 10);
SUPPORT_PARAM_TYPE(unsigned short, 11);
SUPPORT_PARAM_TYPE(unsigned int, 12);
SUPPORT_PARAM_TYPE(long long, 13);
SUPPORT_PARAM_TYPE(unsigned long, 14);
SUPPORT_PARAM_TYPE(long, 15);
SUPPORT_PARAM_TYPE(unsigned long long, 16);
SUPPORT_PARAM_TYPE(float, 17);
SUPPORT_PARAM_TYPE(double, 18);
SUPPORT_PARAM_TYPE(long double, 19);

#undef SUPPORT_PARAM_TYPE

template <typename DataType>
FSimpleParamMessage::FSimpleParamMessage(const DataType& DataValue)
{
	InitWithValue(DataValue);
}

template <typename DataType>
DataType FSimpleParamMessage::Get() const
{
	if (PayloadType == SimpleParamEnumValue<DataType>::Value)
	{
		return *(DataType*)Payload.GetData();
	}
	
	ensureAlwaysMsgf(false,TEXT("FSimpleParamMessage Getter 传入不支持的数据类型！！"));
	return DataType();
}

template <typename DataType>
void FSimpleParamMessage::InitWithValue(const DataType& DataValue)
{
	constexpr uint8 DataTypeID = SimpleParamEnumValue<DataType>::Value;
	PayloadType = DataTypeID;
	
	static_assert(DataTypeID != MAX_uint8,TEXT("FSimpleParamMessage IniWith Value Got Wrong DataType！！"));
	if constexpr (DataTypeID == MAX_uint8)
	{
		PayloadType = DataTypeID;
		return;
	}
	constexpr int32 SizeOfType = sizeof(DataValue);
	Payload.SetNumZeroed(sizeof(DataValue));
	void* Value = Payload.GetData();
	FMemory::Memcpy(Value,&DataValue,SizeOfType);
}

template<>
struct TStructOpsTypeTraits<FSimpleParamMessage> : public TStructOpsTypeTraitsBase2<FSimpleParamMessage>
{
	enum
	{
		WithNetSerializer = true,
	};
};
