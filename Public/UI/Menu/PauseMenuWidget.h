// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SPauseMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPauseMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class AInGameHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnResumeClicked() const;
	FReply OnMultiPlayerClicked() const;
	FReply OnGarageClicked() const;
	FReply OnSettingsClicked() const;
	FReply OnLeaveClicked() const;

	TWeakObjectPtr<class AInGameHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
