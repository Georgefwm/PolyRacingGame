// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/VehicleCustomisationMenuWidget.h"

#include "UI/GarageMenuWidget.h"
#include "UI/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"

#define LOCTEXT_NAMESPACE "garagemenu"

void SVehicleCustomisationMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText TitleText	= LOCTEXT("menu title", "Customiser");
	const FText BackText	= LOCTEXT("Back text", "Back");

	const FText TestTitleText	= LOCTEXT("TestTitleText", "PartName");
	const FText LeftText		= LOCTEXT("LeftText", "<");
	const FText MiddleText		= LOCTEXT("MiddleText", "TEST");
	const FText RightText		= LOCTEXT("RightText", ">");

	
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
				// Option Select starts
			
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(STextBlock)
						.TextStyle(&Style->OptionSelectTitleStyle)
						.Text(TestTitleText)
					]
					
					+ SOverlay::Slot()
					.Padding(Style->OptionSelectTitleMargin)
					[
						SNew(SHorizontalBox)
						
						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.ButtonStyle(&Style->OptionSelectButtonStyle)
							.TextStyle(&Style->OptionSelectTextStyle)
							.Text(LeftText)
							//.OnClicked(this, &SVehicleCustomisationMenuWidget::OnBackClicked)
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.ButtonStyle(&Style->OptionSelectButtonStyle)
							.TextStyle(&Style->OptionSelectTextStyle)
							.Text(MiddleText)
							//.OnClicked(this, &SVehicleCustomisationMenuWidget::OnBackClicked)
						]

						+ SHorizontalBox::Slot()
						[
							SNew(SButton)
							.ButtonStyle(&Style->OptionSelectButtonStyle)
							.TextStyle(&Style->OptionSelectTextStyle)
							.Text(RightText)
							//.OnClicked(this, &SVehicleCustomisationMenuWidget::OnBackClicked)
						]
					]
				]
				// Option Select ends
			]
			

			
			// Back Button
			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Bottom)
			.Padding(Style->BackButtonMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->BackButtonStyle)
				.TextStyle(&Style->BackButtonTextStyle)
				.Text(BackText)
				.OnClicked(this, &SVehicleCustomisationMenuWidget::OnBackClicked)
			]
		];
}

FReply SVehicleCustomisationMenuWidget::OnBackClicked() const
{
	if (!OwningHUD->GarageWidget)
		OwningHUD->GarageWidget = SNew(SGarageMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->GarageWidget.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
