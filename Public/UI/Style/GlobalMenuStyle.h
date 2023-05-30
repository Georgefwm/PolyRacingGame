// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GlobalMenuStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FGlobalStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()
		// Stores a list of Brushes we are using (we aren't using any) into OutBrushes.
	virtual void GetResources(TArray<const FSlateBrush*> & OutBrushes) const override;

	// Stores the TypeName for our widget style.
	static const FName TypeName;

	// Retrieves the type name for our global style, which will be used by our Style Set to load the right file. 
	virtual const FName GetTypeName() const override;
        
	// Allows us to set default values for our various styles. 
	static const FGlobalStyle& GetDefault();

	// Style that defines the text for our menu title. 
	UPROPERTY(EditAnywhere, Category = MenuTitle)
	FTextBlockStyle MenuTitleStyle;

	// Margin for menu title text
	UPROPERTY(EditAnywhere, Category = MenuTitle)
	FMargin TitleTextMargin;

	// Style that define the appearance of all menu buttons. 
	UPROPERTY(EditAnywhere, Category = MenuButton)
	FButtonStyle MenuButtonStyle;

	// Style that defines the text on all of our menu buttons. 
	UPROPERTY(EditAnywhere, Category = MenuButton)
	FTextBlockStyle MenuButtonTextStyle;

	// Margin Separating each button
	UPROPERTY(EditAnywhere, Category = MenuButton)
	FMargin MenuButtonSpacingMargin;
	
	// Margin for whole menu 'list'
	UPROPERTY(EditAnywhere, Category = General)
	FMargin MenuBoxMargin;

	// Text style for selection option slot name. 
	UPROPERTY(EditAnywhere, Category = OptionSelect)
	FTextBlockStyle OptionSelectTitleStyle;

	// Margin for option title text
	UPROPERTY(EditAnywhere, Category = OptionSelect)
	FMargin OptionSelectTitleMargin;

	// Text style for selection option slot name. 
	UPROPERTY(EditAnywhere, Category = OptionSelect)
	FTextBlockStyle OptionSelectTextStyle;

	// Style that define the appearance of all menu buttons. 
	UPROPERTY(EditAnywhere, Category = OptionSelect)
	FButtonStyle OptionSelectButtonStyle;
	

	UPROPERTY(EditAnywhere, Category = MenuActionButton)
	FButtonStyle MenuActionButtonStyle;
	
	UPROPERTY(EditAnywhere, Category = MenuActionButton)
	FTextBlockStyle MenuActionButtonTextStyle;
	
	UPROPERTY(EditAnywhere, Category = MenuActionButton)
	FMargin MenuActionButtonSpacingMargin;

	UPROPERTY(EditAnywhere, Category = MenuActionButton)
	FMargin MenuActionButtonContainerMargin;
};

// Provides a widget style container to allow us to edit properties in-editor
UCLASS(hidecategories = Object, MinimalAPI)
class UGlobalMenuStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	// This is our actual Style object. 
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (ShowOnlyInnerProperties))
	FGlobalStyle MenuStyle;

	// Retrieves the style that this container manages. 
	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return &MenuStyle;
	}

};
