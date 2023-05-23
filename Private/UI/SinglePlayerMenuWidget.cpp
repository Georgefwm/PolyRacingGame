// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SinglePlayerMenuWidget.h"

#include "Components/SizeBox.h"
#include "UI/GlobalMenuStyle.h"
#include "UI/MainMenuWidget.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"
#include "UI/VehicleCustomisationMenuWidget.h"

#define LOCTEXT_NAMESPACE "garagemenu"

void SSinglePlayerMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Garage");
	const FText Slot1Text		= LOCTEXT("Slot1Text", "Vehicle Slot 1");
	
	const FText BackText		= LOCTEXT("Back text", "Back");

	
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
				.Text(Slot1Text)
				.OnClicked(this, &SSinglePlayerMenuWidget::SetSelectedSlot, 0)
			]
		]

		
		// Back Button
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(Style->MenuActionButtonContainerMargin)
		[
			SNew(SBox)
			.WidthOverride(1000.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(Style->MenuActionButtonSpacingMargin)
				.AutoWidth()
				[
					SNew(SBox)
					.HAlign(HAlign_Left)
					[
						SNew(SButton)
						.ButtonStyle(&Style->MenuActionButtonStyle)
						.TextStyle(&Style->MenuActionButtonTextStyle)
						.Text(BackText)
						.OnClicked(this, &SSinglePlayerMenuWidget::OnBackClicked)
					]
				]				
			]			
		]
	];
}

FReply SSinglePlayerMenuWidget::SetSelectedSlot(int DesiredSlot)
{
	VehicleCustomiser->LoadConfiguration(DesiredSlot);
	
	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnEditClicked() const
{
	if (!OwningHUD->MainMenuWidget)
		OwningHUD->MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->MainMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SSinglePlayerMenuWidget::OnBackClicked() const
{
	if (!OwningHUD->MainMenuWidget)
		OwningHUD->MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(OwningHUD);

	// Change the preview to the active selection on exiting menu
	VehicleCustomiser->LoadConfiguration(VehicleCustomiser->ActiveConfigurationSlotIndex);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->MainMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
