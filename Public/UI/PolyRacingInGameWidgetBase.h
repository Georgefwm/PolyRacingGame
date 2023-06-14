// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"


/**
 * 
 */
class SPolyRacingInGameWidgetBase : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPolyRacingInGameWidgetBase) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AInGameHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<class AInGameHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
