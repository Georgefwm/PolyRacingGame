// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/LoadingScreenWidget.h"

#include "UI/MenuHUD.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "loadingscreen"

void SLoadingScreenWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	MapSubsystem = InArgs._MapSubsystem;

	BackgroundImage.Bind(this, &SLoadingScreenWidget::GetBackgroundImage);
	MapName.Bind(this, &SLoadingScreenWidget::GetMapDisplayName);

	/** Text */
	const FText TitleText		= LOCTEXT("TitleText",		"Loading...");
	const FText GameModeText	= LOCTEXT("GameModeText",	"gamemode...");
	
	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(BackgroundImage)
		]

		
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0.f, 100.f, 0.f, 0.f))
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(TitleText)
		]

		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0.f, 200.f, 0.f, 0.f))
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(MapName)
		]
		
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(FMargin(0.f, 0.f, 100.f, 100.f))
		[
			SNew(SThrobber)
			.Animate(SThrobber::All)
		]
	];
}

const FSlateBrush* SLoadingScreenWidget::GetBackgroundImage() const
{
	if (!MapSubsystem.IsValid())
		return new FSlateBrush();

	FSlateBrush* BackgroundImageSlateBrush = new FSlateBrush();
	BackgroundImageSlateBrush->DrawAs = ESlateBrushDrawType::Image;
	BackgroundImageSlateBrush->Tiling = ESlateBrushTileType::NoTile;
	BackgroundImageSlateBrush->ImageSize = FVector2d(1920, 1080);
	
	BackgroundImageSlateBrush->SetResourceObject(MapSubsystem->GetCurrentMapLoadingScreenImage());
	
	return BackgroundImageSlateBrush;
}

FName SLoadingScreenWidget::GetMapDisplayName() const
{
	if (!MapSubsystem.IsValid())
		return FName("");

	return MapSubsystem->GetCurrentMapDisplayName();
}

#undef LOCTEXT_NAMESPACE
