#include "LogSystem/DSLogSystem.h"

DEFINE_LOG_CATEGORY(DSLogBlueprint);



FString UDSLogSystem::GetFinalMessageByConfig_Impl(const FString& Message, UObject* ContextObject)
{
	UDSDebugSystemSetting* setting = UDSDebugSystemSetting::Get();
	if (!setting)
	{
		return TEXT("");
	}

	// 网络模式前缀
	FString netModePrefix = setting->bLogNetMode ? DS_NET_MODE_PREFIX : "";

	// ContextObject Name
	FString objectContextName = "";
	if (setting->bLogContextObject)
	{
		objectContextName = ContextObject ? "  (" + ContextObject->GetName() + ") " : "";
	}

	// 最终日志
	const FString finalMessage = netModePrefix + "--> " + Message + objectContextName;
	return finalMessage;
}

EDSLogOption UDSLogSystem::Imp_GetLogOptionOverrideByConfig(const EDSLogOption& InOption)
{
	if (InOption == EDSLogOption::Disable)
	{
		return InOption;
	}
	if (InOption == EDSLogOption::Default) // 无复写传入默认，使用配置里默认配置
	{
		UDSDebugSystemSetting* setting = UDSDebugSystemSetting::Get();
		if (!setting)
		{
			return InOption;
		}
		return setting->LogOption;
	}
	return InOption;
}

float UDSLogSystem::Imp_GetDisplayTimeOverrideByConfig(const float& DefaultTime)
{
	if (DefaultTime > 0) // 有复写，直接使用复写
	{
		return DefaultTime;
	}
	UDSDebugSystemSetting* setting = UDSDebugSystemSetting::Get();
	if (!setting)
	{
		return DefaultTime;
	}
	// 无复写使用配置的默认时间
	return setting->DisplayTime;
}

FColor UDSLogSystem::Imp_GetColorOverrideByConfig(const FColor& InColor, const EDSLogType& InLogType)
{
	// 有复写直接使用
	if (InColor != FColor())
	{
		return InColor;
	}

	// 读取配置
	UDSDebugSystemSetting* setting = UDSDebugSystemSetting::Get();
	if (!setting)
	{
		return InColor;
	}
	FColor finalColor = FColor::Silver; // 保底

	if (FColor* color = setting->LogColor.Find(InLogType))
	{
		finalColor = *color;
	}

	return finalColor;
}

void UDSLogSystem::DSLogMessage(EDSLogType Type, const FString& Message, FLinearColor InLogColor /*= FLinearColor(0.0, 0.0, 0.0, 0.0)*/, float TimeToDisplay /*= -1.0f*/, bool EditorOnly /*= false*/, EDSLogOption Option /*= EDSLogOption::Default*/, UObject* ContextObject /*= nullptr*/)
{
#if !WITH_EDITOR

	if (EditorOnly)
	{
		return;
	}

#endif

	if (Type == EDSLogType::Error)
	{
		DS_Internal_LogWithParam(DSLogBlueprint, Error, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject);
	}
	else if (Type == EDSLogType::Log)
	{
		DS_Internal_LogWithParam(DSLogBlueprint, Log, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject);
	}
	else if (Type == EDSLogType::Warning)
	{
		DS_Internal_LogWithParam(DSLogBlueprint, Warning, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject);
	}
	else if (Type == EDSLogType::Verbose)
	{
		DS_Internal_LogWithParam(DSLogBlueprint, Verbose, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject);
	}
	else if (Type == EDSLogType::Display)
	{
		DS_Internal_LogWithParam(DSLogBlueprint, Display, Message, Option, TimeToDisplay, InLogColor.ToFColor(false), ContextObject);
	}
}
