// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Style/UiStyles.h"
#include "Slate/SlateGameResources.h"

TSharedPtr<FSlateStyleSet> FUiStyles::UiStyleInstance = NULL;

void FUiStyles::Initialize()
{
	if (!UiStyleInstance.IsValid())
	{
		UiStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*UiStyleInstance);
	}
}

void FUiStyles::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*UiStyleInstance);
	ensure(UiStyleInstance.IsUnique()); 
	UiStyleInstance.Reset();
}

FName FUiStyles::GetStyleSetName()
{
	static FName StyleSetName(TEXT("PolyRacingMenuStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FUiStyles::Create()
{
	TSharedRef<FSlateGameResources> StyleRef = FSlateGameResources::New(FUiStyles::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	return StyleRef;
}

const ISlateStyle& FUiStyles::Get()
{
	return *UiStyleInstance;
}