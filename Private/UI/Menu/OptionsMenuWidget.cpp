// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/OptionsMenuWidget.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/Style/UiStyles.h"


#define LOCTEXT_NAMESPACE "optionsmenu"

void SOptionsMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	PreviousMenu = InArgs._PreviousMenu;
	
	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText TitleText	= LOCTEXT("menu title", "Options");
	const FText BackText	= LOCTEXT("Back text", "Back");

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->TitleTextMargin)
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(TitleText)
			.LineHeightPercentage(2.f)
		]
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->MenuBoxMargin)
		[
			SNew(SVerticalBox)
			// Play Button
			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
			]
			
		]

		
		// Back Button
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(Style->MenuActionButtonContainerMargin)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.Padding(Style->MenuActionButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuActionButtonStyle)
				.TextStyle(&Style->MenuActionButtonTextStyle)
				.Text(BackText)
				.OnClicked(this, &SOptionsMenuWidget::OnBackClicked)
			]
		]
	];
}

FReply SOptionsMenuWidget::OnBackClicked() const
{
	OwningHUD->MenuWidgetContainer.Get()->SetContent(PreviousMenu.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
