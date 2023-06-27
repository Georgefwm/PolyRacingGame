// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AInGameHUD;


/**
 * 
 */
class SPauseMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SPauseMenuWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<AInGameHUD>, OwningHUD)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply OnResumeClicked() const;
	FReply OnMultiPlayerClicked() const;
	FReply OnGarageClicked() const;
	FReply OnSettingsClicked() const;
	FReply OnLeaveClicked() const;

	TWeakObjectPtr<AInGameHUD> OwningHUD;
	const struct FGlobalStyle* Style;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
