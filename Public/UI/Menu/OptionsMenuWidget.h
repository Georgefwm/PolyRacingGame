// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SOptionsMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SOptionsMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TSharedPtr<class SCompoundWidget>, PreviousMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnBackClicked() const;

	TSharedPtr<SCompoundWidget> PreviousMenu;

	TWeakObjectPtr<class AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
