#include "DSGameplayTags.h"
#include "GameplayTagsManager.h"

//定义一个宏，简化创建Tag的代码
#define ADD_NATIVE_TAG(OutTag, TagName, TagComment) \
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment))

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
	ADD_NATIVE_TAG(DSGameTestTags, "DSGame.TestTag", "Test Tag");




	
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
