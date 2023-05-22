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

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Garage");
	const FText CustomiseText	= LOCTEXT("Custom title", "Customise");
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
				.Text(CustomiseText)
				.OnClicked(this, &SGarageMenuWidget::OnCustomiseClicked)
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
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuActionButtonStyle)
				.TextStyle(&Style->MenuActionButtonTextStyle)
				.Text(BackText)
				.OnClicked(this, &SGarageMenuWidget::OnBackClicked)
			]
		]
	];
}

FReply SGarageMenuWidget::OnCustomiseClicked() const
{
	if (!OwningHUD->CustomiseWidget)
		OwningHUD->CustomiseWidget = SNew(SVehicleCustomisationMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->CustomiseWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SGarageMenuWidget::OnBackClicked() const
{
	if (!OwningHUD->MainMenuWidget)
		OwningHUD->MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->MainMenuWidget.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
