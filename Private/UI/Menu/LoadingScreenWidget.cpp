// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/LoadingScreenWidget.h"

#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "loadingscreen"

void SLoadingScreenWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText TitleText = LOCTEXT("GameTitle", "Loading...");
	
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(Style->TitleTextMargin)
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(TitleText)
		]
	];
}

#undef LOCTEXT_NAMESPACE
