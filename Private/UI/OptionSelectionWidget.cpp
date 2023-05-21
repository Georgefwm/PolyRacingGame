// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/OptionSelectionWidget.h"

#include "UI/GlobalMenuStyle.h"
#include "UI/UiStyles.h"

#define LOCTEXT_NAMESPACE "garagemenu"

void SOptionSelectionWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	//OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	Customiser = InArgs._Customiser;
	
	OptionSlotNameString = InArgs._OptionSlotName;
	OptionSlotName = FText::FromString(OptionSlotNameString);
	
	OptionIndex.Bind(this, &SOptionSelectionWidget::GetOptionIndex);
	
	const FText LeftText		= LOCTEXT("LeftText", "<");
	const FText RightText		= LOCTEXT("RightText", ">");

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(Style->OptionSelectTitleMargin)
			[
				SNew(STextBlock)
				.TextStyle(&Style->OptionSelectTitleStyle)
				.Text(OptionSlotName)
				.Justification(ETextJustify::Center)
				.MinDesiredWidth(400.f)
			]
			
			+ SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.25f)
				[
					SNew(SButton)
					.ButtonStyle(&Style->OptionSelectButtonStyle)
					.TextStyle(&Style->OptionSelectTextStyle)
					.Text(LeftText)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this, &SOptionSelectionWidget::OnPreviousClicked)
				]

				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.FillWidth(1.f)
				[
					SNew(STextBlock)
					.TextStyle(&Style->OptionSelectTextStyle)
					.Text(OptionIndex)
					.Justification(ETextJustify::Center)
				]

				+ SHorizontalBox::Slot()
				.FillWidth(0.25f)
				[
					SNew(SButton)
					.ButtonStyle(&Style->OptionSelectButtonStyle)
					.TextStyle(&Style->OptionSelectTextStyle)
					.Text(RightText)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.OnClicked(this, &SOptionSelectionWidget::OnNextClicked)
				]
			]
		]
	];
}

FReply SOptionSelectionWidget::OnNextClicked()
{
	if (Customiser)
		Customiser->SetComponentFromSlotName(OptionSlotNameString, 1);
		
	return FReply::Handled();
}
 
FReply SOptionSelectionWidget::OnPreviousClicked()
{
	if (Customiser)
		Customiser->SetComponentFromSlotName(OptionSlotNameString, -1);
		
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
