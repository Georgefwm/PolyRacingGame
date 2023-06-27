// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UMapSubsystem;

/**
 * 
 */
class SLoadingScreenWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLoadingScreenWidget) {}

	SLATE_ARGUMENT(TWeakObjectPtr<UMapSubsystem>, MapSubsystem)

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
