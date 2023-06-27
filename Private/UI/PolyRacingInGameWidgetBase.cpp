// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PolyRacingInGameWidgetBase.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Style/UiStyles.h"


#define LOCTEXT_NAMESPACE "ingamewidgetbase"

void SPolyRacingInGameWidgetBase::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText TitleText	= LOCTEXT("menu title", "Customiser");
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->TitleTextMargin)
		// [
			// TitleText
			// SNew(STextBlock)
			// .TextStyle(&Style->MenuTitleStyle)
			// .Text(TitleText)
			// .LineHeightPercentage(2.f)
		// ]
	];
}

#undef LOCTEXT_NAMESPACE
