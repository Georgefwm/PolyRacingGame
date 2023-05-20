// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "UI/GarageMenuWidget.h"
#include "UI/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"

#define LOCTEXT_NAMESPACE "mainmenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	
	const FMargin ButtonPadding			= FMargin(0.f, 10.f);

	/** Text */
	const FText TitleText		= LOCTEXT("Game title", "Poly Racing");
	const FText PlayText		= LOCTEXT("PlayButtonText", "Play");
	const FText GarageText		= LOCTEXT("GarageButtonText", "Garage");
	const FText SettingsText	= LOCTEXT("SettingsButtonText", "Settings");
	const FText QuitText		= LOCTEXT("QuitButtonText", "Quit");

	
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
					.Text(PlayText)
					.OnClicked(this, &SMainMenuWidget::OnPlayClicked)
				]

				// Play Button
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(GarageText)
					.OnClicked(this, &SMainMenuWidget::OnGarageClicked)
				]

				// Settings Button
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(SettingsText)
					.OnClicked(this, &SMainMenuWidget::OnSettingsClicked)
				]

				// Quit Button
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(QuitText)
					.OnClicked(this, &SMainMenuWidget::OnQuitClicked)
				]
			]
		];
}

FReply SMainMenuWidget::OnPlayClicked() const
{
	return FReply::Handled();
}

FReply SMainMenuWidget::OnGarageClicked() const
{
	OwningHUD->GarageWidget = SNew(SGarageMenuWidget).OwningHUD(OwningHUD);
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->GarageWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SMainMenuWidget::OnSettingsClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Settings not implemented... yet"));
	
	return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked() const
{
	if (!OwningHUD.IsValid()) return FReply::Handled();

	if (APlayerController* Controller = OwningHUD->PlayerOwner)
	{
		Controller->ConsoleCommand("quit");
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
