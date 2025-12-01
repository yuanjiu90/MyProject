#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include <Common/DSDebugSystem_Def.h>
#include <Common/DSDebugSystemSetting.h>
#include "DSLogSystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DSLogBlueprint, Log, All);

/************************************************************************/
/*                             Utility                                  */
/************************************************************************/

// 当前Class
#define DS_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))
// 当前Function
#define DS_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))
// 当前Line
#define DS_CUR_LINE  (FString::FromInt(__LINE__))
// 当前Class+Line
#define DS_CLASS_LINE_SUFFIX ("  (" + DS_CUR_CLASS + ":" +DS_CUR_LINE + ")")

// 根据配置获取后缀
#define DS_Internal_Get_Suffix_By_Config \
	(!UDSDebugSystemSetting::Get() ? FString("") \
	: UDSDebugSystemSetting::Get()->bLogCodeLine ? DS_CLASS_LINE_SUFFIX \
	: FString(""))
// 网络模式前缀
#define DS_NET_MODE_PREFIX \
	(!GWorld ? FString("") \
	: GWorld->GetNetMode() == NM_Client ? FString("[Client ") + FString::FromInt(UE::GetPlayInEditorID() - 1) + FString("] ") \
	: GWorld->GetNetMode() == NM_ListenServer ? FString("[Listen Server] ") \
	: GWorld->GetNetMode() == NM_DedicatedServer ? FString("[Server] ") \
	: FString(""))


/************************************************************************/
/*                                Log                                   */
/************************************************************************/

// 屏幕打印
#define DS_Internal_Log_To_Viewport(Message, LogType, DisplayTime, DisplayColor) \
	FString msgToPrint = TEXT(""); \
	if(EDSLogType::Warning == EDSLogType::LogType){ msgToPrint = TEXT("Warning: ") + Message; } \
	else if(EDSLogType::Error == EDSLogType::LogType){ msgToPrint = TEXT("Error: ") + Message; } \
	else if(EDSLogType::Display == EDSLogType::LogType){ msgToPrint = TEXT("Display: ") + Message; } \
	if(msgToPrint!=TEXT("")){GEngine->AddOnScreenDebugMessage(-1, DisplayTime, DisplayColor, msgToPrint);} \

// 控制台输出
#define DS_Internal_Log_To_Console(Category, LogType, Message) \
	UE_LOG(Category, LogType, TEXT("%s"), *Message);

// 输出日志到文件
#define DS_Internal_Log_To_File(Message) \
	// todo: ...

// 最终输出日志实现
#define  DS_Internal_LogWithParam(Category, LogType, Message, LogOption, TimeToDisplay, LogColor, ContextObject) \
	EDSLogOption finalLogOption = UDSLogSystem::Imp_GetLogOptionOverrideByConfig(LogOption); \
	if(finalLogOption != EDSLogOption::Disable) \
	{ \
		FString finalLogMsg = UDSLogSystem::GetFinalMessageByConfig_Impl(Message,ContextObject); \
		float finalDisplayTime = UDSLogSystem::Imp_GetDisplayTimeOverrideByConfig(TimeToDisplay); \
		FColor finalDisplayColor = UDSLogSystem::Imp_GetColorOverrideByConfig(LogColor, EDSLogType::LogType); \
		if(finalLogOption == EDSLogOption::Default || finalLogOption == EDSLogOption::Both) \
		{ \
			DS_Internal_Log_To_Console(Category,LogType,finalLogMsg); \
			DS_Internal_Log_To_Viewport(finalLogMsg,LogType,finalDisplayTime,finalDisplayColor); \
		} \
		else if (finalLogOption == EDSLogOption::Viewport) \
		{ \
			DS_Internal_Log_To_Viewport(finalLogMsg, LogType, finalDisplayTime, finalDisplayColor); \
		} \
		else if (finalLogOption == EDSLogOption::Console) \
		{ \
			DS_Internal_Log_To_Console(Category, LogType, finalLogMsg); \
		} \
		DS_Internal_Log_To_File(finalLogMsg) \
	}

/************************************************************************/
/*                              Ensure                                  */
/************************************************************************/

// Ensure 实现
template<typename ConditionToCheck, typename EnsureFunction>
bool DS_Ensure_Impl(ConditionToCheck InEnsureCondition, EnsureFunction EnsureImpl)
{
	if (WITH_EDITOR && UDSDebugSystemSetting::Get() && UDSDebugSystemSetting::Get()->bEnableEnsure)
	{
		if (!InEnsureCondition)
		{
			EnsureImpl();
		}
	}
	if (InEnsureCondition)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Ensure
#define  DS_Internal_Ensure(InEnsureCondition) \
	DS_Ensure_Impl(InEnsureCondition, []()->bool{return ensureAlways(false);})
// EnsureAlwaysMsgf
#define  DS_Internal_EnsureMsgf(InEnsureCondition, FormatString, ...) \
	DS_Ensure_Impl(InEnsureCondition, []()->bool{ \
		FString msgToLog = TEXT("[Ensure Condition Failed] -> ") + FString::Printf(FormatString, ##__VA_ARGS__) + DS_Internal_Get_Suffix_By_Config; \
		DS_Internal_LogWithParam(LogTemp, Error, msgToLog, EDSLogOption::Default, -1, FColor(), nullptr); \
		return ensureAlways(false); \
	})





// 日志系统（Helper方法及定义蓝图接口）
UCLASS()
class DSDEBUGSYSTEM_API UDSLogSystem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// 自定义参数输出日志 （可选参数保持默认则使用配置进行输出）
	UFUNCTION(BlueprintCallable, Category = "DebugLog", meta = (DefaultToSelf = "ContextObject", HidePin = "ContextObject", AdvancedDisplay = "2"))
	static void DSLogMessage(EDSLogType Type, const FString& Message, FLinearColor InLogColor = FLinearColor(0.0, 0.0, 0.0, 0.0), float TimeToDisplay = -1.0f, bool EditorOnly = false, EDSLogOption Option = EDSLogOption::Default, UObject* ContextObject = nullptr);

public:

	// 根据配置最终前缀
	static FString GetFinalMessageByConfig_Impl(const FString& Message, UObject* ContextObject);
	// 根据配置选择是否复写输出模式
	static EDSLogOption Imp_GetLogOptionOverrideByConfig(const EDSLogOption& InOption);
	// 根据配置选择是否复写打印时间
	static float Imp_GetDisplayTimeOverrideByConfig(const float& InTime);
	// 根据配置选择是否复写打印颜色
	static FColor Imp_GetColorOverrideByConfig(const FColor& InColor, const EDSLogType& InLogType);

};