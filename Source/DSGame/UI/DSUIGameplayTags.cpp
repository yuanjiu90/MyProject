#include "DSUIGameplayTags.h"
#include "GameplayTagsManager.h"

TObjectPtr<UDSUIGameplayTags> UDSUIGameplayTags::Singleton = nullptr;

UDSUIGameplayTags& UDSUIGameplayTags::Get()
{
	if (!Singleton)
	{
		Singleton = NewObject<UDSUIGameplayTags>(GetTransientPackage(), UDSUIGameplayTags::StaticClass() ,NAME_None);
		Singleton->InitializeNativeTags();
		Singleton->AddToRoot();//防止该对象被垃圾回收
	}
	check(Singleton);
	return *Singleton;
}

void UDSUIGameplayTags::InitializeNativeTags()
{
	//Layer
	AddNativeTag(UI_Layer_Modal, "UI.Layer.Modal", "This is Modal Layer Tag");
	AddNativeTag(UI_Layer_GameMenu, "UI.Layer.GameMenu", "This is GameMenu Layer Tag");
	AddNativeTag(UI_Layer_Menu, "UI.Layer.Menu", "This is Modal Menu Tag");
	AddNativeTag(UI_Layer_Game, "UI.Layer.Game", "This is Modal Game Tag");
}

void UDSUIGameplayTags::AddNativeTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}

void UDSUIGameplayTags::BeginDestroy()
{
	if(Singleton)
	{
		Singleton->RemoveFromRoot();
		Singleton = nullptr;
	}
	Super::BeginDestroy();
}
