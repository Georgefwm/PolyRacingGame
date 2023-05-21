// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Customisation/VehicleCustomiser.h"


/**
 * 
 */
class SOptionSelectionWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SOptionSelectionWidget) {}

	SLATE_ARGUMENT(TSharedPtr<FVehicleCustomiser>, Customiser)
	SLATE_ARGUMENT(FString, OptionSlotName)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	const struct FGlobalStyle* Style;

	TSharedPtr<FVehicleCustomiser> Customiser;

	FString OptionSlotNameString;
	FText OptionSlotName;

	TAttribute<FText> OptionIndex;
	FText GetOptionIndex() const { return Customiser->GetOptionSlotCurrentIndex(OptionSlotNameString); }

	FReply OnNextClicked();
	FReply OnPreviousClicked();

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
