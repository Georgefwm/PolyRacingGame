// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Subsystem/MapSubsystem.h"

/**
 * 
 */
class SLoadingScreenWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLoadingScreenWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<class UMapSubsystem>, MapSubsystem)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	TWeakObjectPtr<UMapSubsystem> MapSubsystem;
	
	const struct FGlobalStyle* Style;

	TAttribute<const FSlateBrush*> BackgroundImage;
	const FSlateBrush* GetBackgroundImage() const;

	TAttribute<FText> MapName;
	FText GetMapDisplayName() const;

	virtual bool SupportsKeyboardFocus() const override { return true; }
};
