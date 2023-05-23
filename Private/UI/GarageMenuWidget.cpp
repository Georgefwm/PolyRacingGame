// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GarageMenuWidget.h"

#include "UI/GlobalMenuStyle.h"
#include "UI/MainMenuWidget.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"
#include "UI/VehicleCustomisationMenuWidget.h"

#define LOCTEXT_NAMESPACE "garagemenu"

void SGarageMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	
	VehicleCustomiser = FVehicleCustomiser::Get();

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Garage");
	const FText Slot1Text		= LOCTEXT("Slot1Text", "Vehicle Slot 1");
	const FText Slot2Text		= LOCTEXT("Slot2Text", "Vehicle Slot 2");
	const FText Slot3Text		= LOCTEXT("Slot3Text", "Vehicle Slot 3");
	const FText Slot4Text		= LOCTEXT("Slot4Text", "Vehicle Slot 4");
	const FText Slot5Text		= LOCTEXT("Slot5Text", "Vehicle Slot 5");

	
	const FText BackText		= LOCTEXT("Back text", "Back");
	const FText EditText		= LOCTEXT("Edit text", "Edit");
	const FText SetActiveText	= LOCTEXT("Set Active text", "Set Active");

	
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


		// TEMPORARY - for testing child menus
		// TODO: Implement this menu + vehicle slot functionality
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
				.OnClicked(this, &SGarageMenuWidget::SetSelectedSlot, 0)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(Slot2Text)
				.OnClicked(this, &SGarageMenuWidget::SetSelectedSlot, 1)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(Slot3Text)
				.OnClicked(this, &SGarageMenuWidget::SetSelectedSlot, 2)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(Slot4Text)
				.OnClicked(this, &SGarageMenuWidget::SetSelectedSlot, 3)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(Slot5Text)
				.OnClicked(this, &SGarageMenuWidget::SetSelectedSlot, 4)
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
				.OnClicked(this, &SGarageMenuWidget::OnBackClicked)
			]

			+ SHorizontalBox::Slot()
			.Padding(Style->MenuActionButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuActionButtonStyle)
				.TextStyle(&Style->MenuActionButtonTextStyle)
				.Text(EditText)
				.OnClicked(this, &SGarageMenuWidget::OnEditClicked)
			]
			
			+ SHorizontalBox::Slot()
			.Padding(Style->MenuActionButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuActionButtonStyle)
				.TextStyle(&Style->MenuActionButtonTextStyle)
				.Text(SetActiveText)
				.OnClicked(this, &SGarageMenuWidget::OnSetActiveClicked)
			]
		]
	];
}

FReply SGarageMenuWidget::SetSelectedSlot(int DesiredSlot)
{
	VehicleCustomiser->LoadConfiguration(DesiredSlot);
	
	return FReply::Handled();
}

FReply SGarageMenuWidget::OnEditClicked() const
{
	if (!OwningHUD->CustomiseWidget)
		OwningHUD->CustomiseWidget = SNew(SVehicleCustomisationMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->CustomiseWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SGarageMenuWidget::OnSetActiveClicked() const
{
	VehicleCustomiser->ActiveConfigurationSlotIndex = VehicleCustomiser->CurrentConfigurationIndex;

	return FReply::Handled();
}

FReply SGarageMenuWidget::OnBackClicked() const
{
	if (!OwningHUD->MainMenuWidget)
		OwningHUD->MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(OwningHUD);

	// Change the preview to the active selection on exiting menu
	VehicleCustomiser->LoadConfiguration(VehicleCustomiser->ActiveConfigurationSlotIndex);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->MainMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
