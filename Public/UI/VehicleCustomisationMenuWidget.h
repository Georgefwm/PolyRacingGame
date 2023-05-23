// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Customisation/VehicleCustomiser.h"


/**
 * 
 */
class SVehicleCustomisationMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SVehicleCustomisationMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnBackClicked();
	FReply OnSaveClicked();

	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	TSharedPtr<FVehicleCustomiser> VehicleCustomiser;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
