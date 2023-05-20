// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionsMenuWidget.h"

#include "UI/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"

#define LOCTEXT_NAMESPACE "optionsmenu"

void SOptionsMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	
	const FMargin ButtonPadding			= FMargin(0.f, 10.f);

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Options");

	
	ChildSlot
		[
			SNew(SOverlay)

			// Useful when working on the UI layout
			//
			// + SOverlay::Slot()
			// .HAlign(HAlign_Fill)
			// .VAlign(VAlign_Fill)
			// [
			// 	// Black Background
			// 	SNew(SImage)
			// 	.ColorAndOpacity(FColor::Black)
			// ]

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
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
				]

				
			]
		];
}

FReply SOptionsMenuWidget::OnBackClicked() const
{
	if (!OwningHUD.IsValid()) return FReply::Handled();

	if (APlayerController* Controller = OwningHUD->PlayerOwner)
	{
		Controller->ConsoleCommand("quit");
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
