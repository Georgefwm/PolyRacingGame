// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Customisation/VehicleCustomiser.h"


/**
 * 
 */
class SSinglePlayerMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SSinglePlayerMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnRaceClicked() const;
	FReply OnFreeRoamClicked() const;
	FReply OnBackClicked() const;

	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
