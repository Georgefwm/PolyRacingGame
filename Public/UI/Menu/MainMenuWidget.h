// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AMenuHUD;
class SCompoundWidget;


/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMainMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AMenuHUD>, OwningHUD)
	SLATE_ARGUMENT(TSharedPtr<SCompoundWidget>, PreviousMenu)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnSinglePlayerClicked() const;
	FReply OnMultiPlayerClicked() const;
	FReply OnGarageClicked() const;
	FReply OnSettingsClicked() const;
	FReply OnQuitClicked() const;
	
	TSharedPtr<SCompoundWidget> PreviousMenu;

	TWeakObjectPtr<AMenuHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
