#pragma once
#include "DSLogSystem.h"


/************************************************************************/
/*                           DSLogHelper                                */
/* 基于UE_LOG的封装														*/
/* 添加ProjectSetting控制各类输出模式										*/
/* LogError自动Ensure													*/
/* 屏蔽DedicatedServer的Ensure，避免服务端报错时卡顿，但正常输出Error			*/
/* 输出Log至独立文件(todo...)												*/
/************************************************************************/



/**
*
* 根据配置选择是否激活ensure
*
* e.g.
*
* DSEnsure(true);
* if(DSEnsure(true)) { // Do Somethings... }
*
* DSEnsureMsgf(false, TEXT("Hit Ensure"));
* if(DSEnsureMsgf(false, TEXT("Hit Ensure"))) { // Do Somethings... }
* 
*/

#define DSEnsure(InEnsureCondition)		DS_Internal_Ensure(InEnsureCondition)

#define DSEnsureMsgf(InEnsureCondition, FormatString, ...) DS_Internal_EnsureMsgf(InEnsureCondition, FormatString, ##__VA_ARGS__ )



/**
 *
 * 根据默认配置输出Log
 *
 * e.g.
 * DSLog(LogTemp, Log, TEXT("Test Log"));
 * DSLog(LogTemp, Error, TEXT("Error Message : %s"), TEXT("Test Log"));
 *
 * FString TestLog = TEXT("Test Log String");
 * DSLogString(LogTemp, Warning, TEXT("%s), *TestLog);
 *
 */

#define DSLog(LogCategory, LogType, FormatString, ...) \
	DSLogWithParam(LogCategory, LogType, FString::Printf(FormatString, ##__VA_ARGS__ ), EDSLogOption::Default);

#define DSLogTemp(LogType, FormatString, ...) \
	DSLogWithParam(LogTemp, LogType, FString::Printf(FormatString, ##__VA_ARGS__ ), EDSLogOption::Default);

#define DSLogToConsole(LogCategory, LogType, FormatString, ...) \
	DSLogWithParam(LogCategory, LogType, FString::Printf(FormatString, ##__VA_ARGS__ ), EDSLogOption::Console);



 /**
  *
  * 根据自定义LogOption参数输出日志
  *
  * e.g.
  * FString TestLog = TEXT("Test Log String");
  * DSLogWithParam(LogTemp, Log, TestLog, EDSLogOption::Console);
  *
  */
#define DSLogWithParam(Category, LogType, MessageString, LogOption) \
	{ \
		FString msgToLog = MessageString + DS_Internal_Get_Suffix_By_Config; \
		DS_Internal_LogWithParam(Category, LogType, msgToLog, LogOption, -1, FColor(),nullptr); \
		if(EDSLogType::LogType == EDSLogType::Error) \
		{ \
			if(WITH_EDITOR && UDSDebugSystemSetting::Get() && UDSDebugSystemSetting::Get()->bEnableEnsure) \
			{ \
				ensureAlways(false); \
			} \
		} \
	}



/**
 *
 * 快速声明根据LogCategory对应的蓝图接口
 *
 */
#define DSDeclareLogCategoryBlueprintNode(CategoryName) \
	UFUNCTION(BlueprintCallable, Category = "DebugLog", meta = (DefaultToSelf = "ContextObject", HidePin = "ContextObject", AdvancedDisplay = "2")) \
	static void DSLogMessage_##CategoryName(EDSLogType Type, const FString & Message, FLinearColor InLogColor = FLinearColor(0.0, 0.0, 0.0, 0.0), float TimeToDisplay = -1.0f, bool EditorOnly = false, EDSLogOption Option = EDSLogOption::Default, UObject * ContextObject = nullptr) \
	{ \
		if(!WITH_EDITOR && EditorOnly) {return;} \
		if (Type == EDSLogType::Error) \
		{ \
			Internal_DSLogWithParam(CategoryName, Error, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject); \
		} \
		else if (Type == EDSLogType::Log) \
		{ \
			Internal_DSLogWithParam(CategoryName, Log, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject); \
		} \
		else if (Type == EDSLogType::Warning) \
		{ \
			Internal_DSLogWithParam(CategoryName, Warning, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject); \
		} \
	}
