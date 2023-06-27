// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AMenuHUD;
class SCompoundWidget;

/**
 * 
 */
class SSinglePlayerMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SSinglePlayerMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TSharedPtr<SCompoundWidget>, PreviousMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnRaceClicked() const;
	FReply OnTimeTrialClicked() const;
	FReply OnFreeRoamClicked() const;
	FReply OnBackClicked() const;

	TSharedPtr<SCompoundWidget> PreviousMenu;
	
	TWeakObjectPtr<AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
