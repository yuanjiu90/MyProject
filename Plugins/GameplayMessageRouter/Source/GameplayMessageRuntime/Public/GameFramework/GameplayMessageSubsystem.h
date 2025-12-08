// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/GameInstanceSubsystem.h"
#include "GameFramework/GameplayMessageTypes2.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "DSExtends/SimpleParamWrapperMessage.h"
#include "Logging/LogMacros.h"

#include "GameplayMessageSubsystem.generated.h"

class UGameplayMessageSubsystem;
GAMEPLAYMESSAGERUNTIME_API DECLARE_LOG_CATEGORY_EXTERN(LogGameplayMessageSubsystem, Log, All);

class UAsyncAction_ListenForGameplayMessage;

/**
 * An opaque handle that can be used to remove a previously registered message listener
 * @see UGameplayMessageSubsystem::RegisterListener and UGameplayMessageSubsystem::UnregisterListener
 */
USTRUCT(BlueprintType)
struct GAMEPLAYMESSAGERUNTIME_API FGameplayMessageListenerHandle
{
public:
	GENERATED_BODY()

	FGameplayMessageListenerHandle() {}

	void Unregister();

	bool IsValid() const { return ID != 0; }

	int32 GetID() const { return ID; }

	FGameplayTag GetChannel() const { return Channel; }

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UGameplayMessageSubsystem> Subsystem;

	UPROPERTY(Transient)
	FGameplayTag Channel;

	UPROPERTY(Transient)
	int32 ID = 0;

	FDelegateHandle StateClearedHandle;

	friend UGameplayMessageSubsystem;

	FGameplayMessageListenerHandle(UGameplayMessageSubsystem* InSubsystem, FGameplayTag InChannel, int32 InID) : Subsystem(InSubsystem), Channel(InChannel), ID(InID) {}
};

/** 
 * Entry information for a single registered listener
 */
USTRUCT()
struct FGameplayMessageListenerData
{
	GENERATED_BODY()

	// Callback for when a message has been received
	TFunction<void(FGameplayTag, const UScriptStruct*, const void*)> ReceivedCallback;

	int32 HandleID;
	EGameplayMessageMatch MatchType;

	// Adding some logging and extra variables around some potential problems with this
	TWeakObjectPtr<const UScriptStruct> ListenerStructType = nullptr;
	bool bHadValidType = false;
};

/**
 * This system allows event raisers and listeners to register for messages without
 * having to know about each other directly, though they must agree on the format
 * of the message (as a USTRUCT() type).
 *
 *
 * You can get to the message router from the game instance:
 *    UGameInstance::GetSubsystem<UGameplayMessageSubsystem>(GameInstance)
 * or directly from anything that has a route to a world:
 *    UGameplayMessageSubsystem::Get(WorldContextObject)
 *
 * Note that call order when there are multiple listeners for the same channel is
 * not guaranteed and can change over time!
 */
UCLASS()
class GAMEPLAYMESSAGERUNTIME_API UGameplayMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend UAsyncAction_ListenForGameplayMessage;

public:

	/**
	 * @return the message router for the game instance associated with the world of the specified object
	 */
	static UGameplayMessageSubsystem& Get(const UObject* WorldContextObject);

	/**
	 * @return true if a valid GameplayMessageRouter subsystem if active in the provided world
	 */
	static bool HasInstance(const UObject* WorldContextObject);

	//~USubsystem interface
	virtual void Deinitialize() override;
	//~End of USubsystem interface

	/**
	 * Broadcast a message on the specified channel
	 *
	 * @param Channel			The message channel to broadcast on
	 * @param Message			The message to send (must be the same type of UScriptStruct expected by the listeners for this channel, otherwise an error will be logged)
	 * 注 ： Message 已经扩展为接受 
	 * + int , float , double ,bool 四种类型使用 FSimpleParamMessage 结构体来传输， Listen 的时候请使用 RegisterListenerWithSimpleParam 来获取
	 */
	template <typename MessageType, bool InstancedStructAsOrigin = false>
	void BroadcastMessage(FGameplayTag Channel, const MessageType& Message)
	{
		if constexpr (std::is_same_v<MessageType,FInstancedStruct>)
		{
			// 传递的是 FInstancedStruct
			if constexpr (InstancedStructAsOrigin)
			{
				BroadcastMessageInternal(Channel, FInstancedStruct::StaticStruct() , &Message);
			}
			else
			{
				BroadcastMessageInternal(Channel, Message.GetScriptStruct() , Message.GetMemory());
			}
		}
		else if constexpr (std::is_arithmetic_v<MessageType>)
		{
			const UScriptStruct* StructType = FSimpleParamMessage::StaticStruct();
			FSimpleParamMessage TempMessage(Message);
			BroadcastMessageInternal(Channel, StructType, &TempMessage);
		}
		else
		{
			const UScriptStruct* StructType = TBaseStructure<MessageType>::Get();
			BroadcastMessageInternal(Channel, StructType, &Message);
		}
	}

	/**
	 * Register to receive messages on a specified channel
	 *
	 * @param Channel			The message channel to listen to
	 * @param Callback			Function to call with the message when someone broadcasts it (must be the same type of UScriptStruct provided by broadcasters for this channel, otherwise an error will be logged)
	 * @param MatchType
	 *
	 * @return a handle that can be used to unregister this listener (either by calling Unregister() on the handle or calling UnregisterListener on the router)
	 */
	template <typename FMessageStructType>
	FGameplayMessageListenerHandle RegisterListener(FGameplayTag Channel, TFunction<void(FGameplayTag, const FMessageStructType&)>&& Callback, EGameplayMessageMatch MatchType = EGameplayMessageMatch::ExactMatch)
	{
		auto ThunkCallback = [InnerCallback = MoveTemp(Callback)](FGameplayTag ActualTag, const UScriptStruct* SenderStructType, const void* SenderPayload)
		{
			InnerCallback(ActualTag, *reinterpret_cast<const FMessageStructType*>(SenderPayload));
		};

		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		return RegisterListenerInternal(Channel, ThunkCallback, StructType, MatchType);
	}

	/**
	 * Register to receive messages on a specified channel and handle it with a specified member function
	 * Executes a weak object validity check to ensure the object registering the function still exists before triggering the callback
	 *
	 * @param Channel			The message channel to listen to
	 * @param Object			The object instance to call the function on
	 * @param Function			Member function to call with the message when someone broadcasts it (must be the same type of UScriptStruct provided by broadcasters for this channel, otherwise an error will be logged)
	 *
	 * @return a handle that can be used to unregister this listener (either by calling Unregister() on the handle or calling UnregisterListener on the router)
	 */
	template <typename FMessageStructType, typename TOwner = UObject>
	FGameplayMessageListenerHandle RegisterListener(FGameplayTag Channel, TOwner* Object, void(TOwner::* Function)(FGameplayTag, const FMessageStructType&))
	{
		TWeakObjectPtr<TOwner> WeakObject(Object);
		return RegisterListener<FMessageStructType>(Channel,
			[WeakObject, Function](FGameplayTag Channel, const FMessageStructType& Payload)
			{
				if (TOwner* StrongObject = WeakObject.Get())
				{
					(StrongObject->*Function)(Channel, Payload);
				}
			});
	}

	/**
	 * Register to receive messages on a specified channel with extra parameters to support advanced behavior
	 * The stateful part of this logic should probably be separated out to a separate system
	 *
	 * @param Channel			The message channel to listen to
	 * @param Params			Structure containing details for advanced behavior
	 *
	 * @return a handle that can be used to unregister this listener (either by calling Unregister() on the handle or calling UnregisterListener on the router)
	 */
	template <typename FMessageStructType>
	FGameplayMessageListenerHandle RegisterListener(FGameplayTag Channel, FGameplayMessageListenerParams<FMessageStructType>& Params)
	{
		FGameplayMessageListenerHandle Handle;

		// Register to receive any future messages broadcast on this channel
		if (Params.OnMessageReceivedCallback)
		{
			auto ThunkCallback = [InnerCallback = Params.OnMessageReceivedCallback](FGameplayTag ActualTag, const UScriptStruct* SenderStructType, const void* SenderPayload)
			{
				InnerCallback(ActualTag, *reinterpret_cast<const FMessageStructType*>(SenderPayload));
			};

			const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
			Handle = RegisterListenerInternal(Channel, ThunkCallback, StructType, Params.MatchType);
		}

		return Handle;
	}

	FGameplayMessageListenerHandle RegisterListenerWithSimpleParam(FGameplayTag Channel ,FGameplayMessageListenerParams<FSimpleParamMessage>& Params)
	{
		return RegisterListener(Channel,Params);
	}

	FGameplayMessageListenerHandle RegisterListenerWithNoPayload(FGameplayTag Channel ,FGameplayMessageListenerParams<FSimpleParamMessage>& Params)
	{
		return RegisterListener(Channel,Params);
	}

	template <typename FMessageStructType,typename TOwner = UObject>
	static FGameplayMessageListenerHandle __StaticRegisterMessageListener(FGameplayTag Channel, TOwner* Owner, void(TOwner::* Callback)(FGameplayTag, const FMessageStructType&), EGameplayMessageMatch MatchType)
	{
		UGameplayMessageSubsystem& MessageSubsystem = Get(Owner);
		if(!HasInstance(Owner))
		{
			UE_LOG(LogGameplayMessageSubsystem,Error,TEXT("Cannot Get UGameplayMessageSubsystem ! Maybe not created or destroyed"));
			return FGameplayMessageListenerHandle();
		}
		FGameplayMessageListenerParams<FMessageStructType> Params;
		Params.MatchType = MatchType;
		Params.SetMessageReceivedCallback(Owner,Callback );
		return MessageSubsystem.RegisterListener<FMessageStructType>(Channel,Params);
	}

#define REGISTER_GAMEPLAY_MESSAGE_LISTENER_WITH_MATCH_TYPE(Channel,MessageType,Owner,CallBack,InMatchType)\
	UGameplayMessageSubsystem::__StaticRegisterMessageListener<MessageType>(Channel,Owner,CallBack,InMatchType);

#define REGISTER_GAMEPLAY_MESSAGE_LISTENER(Channel,MessageType,Owner,CallBack)\
	REGISTER_GAMEPLAY_MESSAGE_LISTENER_WITH_MATCH_TYPE(Channel,MessageType,Owner,CallBack,EGameplayMessageMatch::ExactMatch);\

#define REGISTER_GAMEPLAY_MESSAGE_LISTENER_NO_PAYLOAD_WITH_MATCH_TYPE(Channel,Owner,CallBack,MatchType)\
	REGISTER_GAMEPLAY_MESSAGE_LISTENER_WITH_MATCH_TYPE(Channel,FSimpleParamMessage,Owner,CallBack,MatchType);\

#define REGISTER_GAMEPLAY_MESSAGE_LISTENER_NO_PAYLOAD(Channel,Owner,CallBack)\
	REGISTER_GAMEPLAY_MESSAGE_LISTENER_NO_PAYLOAD_WITH_MATCH_TYPE(Channel,Owner,CallBack,EGameplayMessageMatch::ExactMatch);\

	/**
	 * Remove a message listener previously registered by RegisterListener
	 *
	 * @param Handle	The handle returned by RegisterListener
	 */
	void UnregisterListener(FGameplayMessageListenerHandle Handle);

	// 发送空消息
	UFUNCTION(BlueprintCallable, Category = "MessageSubsystem")
	void BroadcastMessageWithNoPayload(FGameplayTag Channel);

protected:
	/**
	 * Broadcast a message on the specified channel
	 *
	 * @param Channel			The message channel to broadcast on
	 * @param Message			The message to send (must be the same type of UScriptStruct expected by the listeners for this channel, otherwise an error will be logged)
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category=Messaging, meta=(CustomStructureParam="Message", AllowAbstract="false", DisplayName="Broadcast Message"))
	void K2_BroadcastMessage(FGameplayTag Channel, const int32& Message);

	DECLARE_FUNCTION(execK2_BroadcastMessage);



private:
	// Internal helper for broadcasting a message
	void BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* StructType, const void* MessageBytes);

	// Internal helper for registering a message listener
	FGameplayMessageListenerHandle RegisterListenerInternal(
		FGameplayTag Channel, 
		TFunction<void(FGameplayTag, const UScriptStruct*, const void*)>&& Callback,
		const UScriptStruct* StructType,
		EGameplayMessageMatch MatchType);

	void UnregisterListenerInternal(FGameplayTag Channel, int32 HandleID);

private:
	// List of all entries for a given channel
	struct FChannelListenerList
	{
		TArray<FGameplayMessageListenerData> Listeners;
		int32 HandleID = 0;
	};

private:
	TMap<FGameplayTag, FChannelListenerList> ListenerMap;
};
