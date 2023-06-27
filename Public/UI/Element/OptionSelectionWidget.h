// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UVehicleCustomiser;

/**
 * 
 */
class SOptionSelectionWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SOptionSelectionWidget) {}

	SLATE_ARGUMENT(UVehicleCustomiser*, Customiser)
	SLATE_ARGUMENT(FString, OptionSlotName)
	
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	const struct FGlobalStyle* Style;

	UVehicleCustomiser* Customiser;

	FString OptionSlotNameString;
	FText OptionSlotName;

	TAttribute<FText> OptionIndex;
	FText GetOptionIndex() const;

	FReply OnNextClicked();
	FReply OnPreviousClicked();

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
