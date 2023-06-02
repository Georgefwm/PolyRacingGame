// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/VehicleCustomisationMenuWidget.h"

#include "UI/Menu/GarageMenuWidget.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/Element/OptionSelectionWidget.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "vehiclecustomisationmenu"

void SVehicleCustomisationMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	PreviousMenu = InArgs._PreviousMenu;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText TitleText	= LOCTEXT("menu title", "Customiser");
	const FText BackText	= LOCTEXT("Back text", "Back");
	const FText SaveText	= LOCTEXT("save text", "Save");

	const FText TestTitleText	= LOCTEXT("TestTitleText", "PartName");
	const FText LeftText		= LOCTEXT("LeftText", "<");
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
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SOptionSelectionWidget)
				.Customiser(OwningHUD->VehicleCustomiser)
				.OptionSlotName(TEXT("VehicleType"))				
			]
			
			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SOptionSelectionWidget)
				.Customiser(OwningHUD->VehicleCustomiser)
				.OptionSlotName(TEXT("Preset"))				
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SOptionSelectionWidget)
				.Customiser(OwningHUD->VehicleCustomiser)
				.OptionSlotName(TEXT("PrimaryColor"))				
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SOptionSelectionWidget)
				.Customiser(OwningHUD->VehicleCustomiser)
				.OptionSlotName(TEXT("AccentColor"))				
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
						.OnClicked(this, &SVehicleCustomisationMenuWidget::OnBackClicked)
					]
				]

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
						.Text(SaveText)
						.OnClicked(this, &SVehicleCustomisationMenuWidget::OnSaveClicked)
					]
				]
			]
		]
	];
}

FReply SVehicleCustomisationMenuWidget::OnSaveClicked()
{
	OwningHUD->VehicleCustomiser->SaveConfiguration(OwningHUD->VehicleCustomiser->CurrentConfigurationIndex);
	return FReply::Handled();
}

FReply SVehicleCustomisationMenuWidget::OnBackClicked()
{
	OwningHUD->MenuWidgetContainer.Get()->SetContent(PreviousMenu.ToSharedRef());
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
