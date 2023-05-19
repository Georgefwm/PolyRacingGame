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
	UPROPERTY(EditAnywhere, Category = TitleText)
	FTextBlockStyle MenuTitleStyle;

	// Margin for menu title text
	UPROPERTY(EditAnywhere, Category = TitleText)
	FMargin TitleTextMargin;

	// Style that define the appearance of all menu buttons. 
	UPROPERTY(EditAnywhere, Category = Buttons)
	FButtonStyle MenuButtonStyle;

	// Style that defines the text on all of our menu buttons. 
	UPROPERTY(EditAnywhere, Category = Buttons)
	FTextBlockStyle MenuButtonTextStyle;
	
	// Margin for whole menu 'list'
	UPROPERTY(EditAnywhere, Category = General)
	FMargin MenuBoxMargin;
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
