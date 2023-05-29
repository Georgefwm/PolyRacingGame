// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "UI/SinglePlayerMenuWidget.h"
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

	/** Text */
	const FText TitleText			= LOCTEXT("GameTitle", "Poly Racing");
	const FText SinglePlayerText	= LOCTEXT("SinglePlayerText", "Single Player");
	const FText MultiPlayerText		= LOCTEXT("MultiPlayerText", "Multiplayer");
	const FText GarageText			= LOCTEXT("GarageButtonText", "Garage");
	const FText SettingsText		= LOCTEXT("SettingsButtonText", "Settings");
	const FText QuitText			= LOCTEXT("QuitButtonText", "Quit");

	
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
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(SinglePlayerText)
					.OnClicked(this, &SMainMenuWidget::OnSinglePlayerClicked)
				]

				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(MultiPlayerText)
					.OnClicked(this, &SMainMenuWidget::OnMultiPlayerClicked)
				]
				
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(GarageText)
					.OnClicked(this, &SMainMenuWidget::OnGarageClicked)
				]

				// Settings Button
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(SettingsText)
					.OnClicked(this, &SMainMenuWidget::OnSettingsClicked)
				]

				// Quit Button
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
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

FReply SMainMenuWidget::OnSinglePlayerClicked() const
{
	if (!OwningHUD->SinglePlayerMenuWidget)
		OwningHUD->SinglePlayerMenuWidget = SNew(SSinglePlayerMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->SinglePlayerMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SMainMenuWidget::OnMultiPlayerClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Multiplayer not implemented... yet"));
	
	return FReply::Handled();
}

FReply SMainMenuWidget::OnGarageClicked() const
{
	if (!OwningHUD->GarageWidget)
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
