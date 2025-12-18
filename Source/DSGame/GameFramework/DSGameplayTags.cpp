#include "DSGameplayTags.h"
#include "GameplayTagsManager.h"

TObjectPtr<UDSGameplayTags> UDSGameplayTags::Singleton = nullptr;

UDSGameplayTags& UDSGameplayTags::Get()
{
	if (!Singleton)
	{
		Singleton = NewObject<UDSGameplayTags>(GetTransientPackage(), UDSGameplayTags::StaticClass() ,NAME_None);
		Singleton->InitializeNativeTags();
		Singleton->AddToRoot();//防止该对象被垃圾回收
	}
	check(Singleton);
	return *Singleton;
}

// 初始化Tag
void UDSGameplayTags::InitializeNativeTags()
{
	//测试代码
	AddNativeTag(DSGameTestTags, "DSGame.TestTag", "This is Test Tag");



	
}

void UDSGameplayTags::AddNativeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

// 销毁
void UDSGameplayTags::BeginDestroy()
{
	if(Singleton)
	{
		Singleton->RemoveFromRoot();
		Singleton = nullptr;
	}
	Super::BeginDestroy();
}
