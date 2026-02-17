// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/UISubsystem.h"
#include "Engine/AssetManager.h"
#include "Widgets/ConfirmScreenWidget.h"
#include "Tags/UIGameplayTags.h"
#include "FrontendFunctionLibrary.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UUISubsystem* UUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
		if (World)
		{
			UUISubsystem* Subsystem = World->GetGameInstance()->GetSubsystem<UUISubsystem>();
			return Subsystem;
		}
	}

	return nullptr;
}

bool UUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}

	return false;
}

void UUISubsystem::RegisterPrimeLayoutWidget(UPrimaryLayoutWidget* LayoutWidgetToSet)
{
	check(LayoutWidgetToSet);
	LayoutWidget = LayoutWidgetToSet;
}

void UUISubsystem::PushConfirmScreenToModalStackAsync(EConfirmScreenType ScreenType, const FText& ScreenTitle, const FText& ScreenMessage, TFunction<void(EConfirmScreenButtonType)> ButtonCallback)
{
	UConfirmInfoObject* ConfirmInfoObj{};

	switch (ScreenType)
	{
	case EConfirmScreenType::Ok:
		ConfirmInfoObj = UConfirmInfoObject::CreateOKScreen(ScreenTitle,ScreenMessage);
		break;

	case EConfirmScreenType::YesOrNo:
		ConfirmInfoObj = UConfirmInfoObject::CreateYesOrNoScreen(ScreenTitle, ScreenMessage);
		break;

	case EConfirmScreenType::OkOrCancle:
		ConfirmInfoObj = UConfirmInfoObject::CreateOkOrCancelScreen(ScreenTitle, ScreenMessage);
		break;

	case EConfirmScreenType::Unknown:
		break;
	default:
		break;
	}
	check(ConfirmInfoObj);

	PushSoftWidgetToStackAsync(
		UIGameplayTags::UI_WidgetStack_Modal, 
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(UIGameplayTags::UI_Widget_ConfirmScreen),
		[ConfirmInfoObj, ButtonCallback](EAsyncPushWidgetState PushState, UAdvancedActivatableWidget* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UConfirmScreenWidget* CreatedConfirmScreen = CastChecked<UConfirmScreenWidget>(PushedWidget);
				CreatedConfirmScreen->InitComfirmScreen(ConfirmInfoObj, ButtonCallback);
			}
		}
	);

}

void UUISubsystem::PushSoftWidgetToStackAsync(const FGameplayTag& WidgetStackTag, TSoftClassPtr<UAdvancedActivatableWidget> SoftWidgetClass,TFunction<void(EAsyncPushWidgetState, UAdvancedActivatableWidget*)> ASyncPushStateCallback)
{
	//name is too long, Activatable widget stack is taked when async action ends,and lambda pushes 
	typedef UCommonActivatableWidgetContainerBase WidgetStack;

	check(!SoftWidgetClass.IsNull());
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad
	(
		SoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda
		(
			[SoftWidgetClass, WidgetStackTag, ASyncPushStateCallback, this]()
			{
				UClass* LoadedWidgetClass = SoftWidgetClass.Get();
				check(LoadedWidgetClass && LayoutWidget);

				WidgetStack* Stack = LayoutWidget->GetWidgetStackWithTag(WidgetStackTag);
				UAdvancedActivatableWidget* CreatedWidget = Stack->AddWidget<UAdvancedActivatableWidget>(
					LoadedWidgetClass,
					[ASyncPushStateCallback](UAdvancedActivatableWidget& CreatedWidgetInstance)
					{
						ASyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				ASyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{

}

void UUISubsystem::Deinitialize()
{

}
