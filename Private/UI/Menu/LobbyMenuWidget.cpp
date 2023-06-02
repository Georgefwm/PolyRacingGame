// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/LobbyMenuWidget.h"

#include "Framework/PolyRacingSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Style/GlobalMenuStyle.h"
#include "UI/Menu/MainMenuWidget.h"
#include "UI/MenuHUD.h"
#include "UI/Menu/GarageMenuWidget.h"
#include "UI/Style/UiStyles.h"

#define LOCTEXT_NAMESPACE "multiplayermenu"

void SLobbyMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");
	

	/** Text */
	const FText TitleText		= LOCTEXT("menu title", "Lobby");
	
	const FText GarageText	= LOCTEXT("GarageText", "Garage");
	const FText VetoMapText	= LOCTEXT("VetoMapText", "Veto Map");
	
	const FText BackText		= LOCTEXT("Back text", "Back");

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->TitleTextMargin)
		[
			// TitleText
			SNew(STextBlock)
			.TextStyle(&Style->MenuTitleStyle)
			.Text(TitleText)
			.LineHeightPercentage(2.f)
		]
		
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(Style->MenuBoxMargin)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(GarageText)
				.OnClicked(this, &SLobbyMenuWidget::OnGarageClicked)
			]

			+ SVerticalBox::Slot()
			.Padding(Style->MenuButtonSpacingMargin)
			[
				SNew(SButton)
				.ButtonStyle(&Style->MenuButtonStyle)
				.TextStyle(&Style->MenuButtonTextStyle)
				.Text(VetoMapText)
				.OnClicked(this, &SLobbyMenuWidget::OnVetoMapClicked)
			]
		]
		
		// Back Button
		+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(Style->MenuActionButtonContainerMargin)
		[
			SNew(SBox)
			.WidthOverride(1000.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(Style->MenuActionButtonSpacingMargin)
				.AutoWidth()
				[
					SNew(SBox)
					.HAlign(HAlign_Left)
					[
						SNew(SButton)
						.ButtonStyle(&Style->MenuActionButtonStyle)
						.TextStyle(&Style->MenuActionButtonTextStyle)
						.Text(BackText)
						.OnClicked(this, &SLobbyMenuWidget::OnBackClicked)
					]
				]				
			]			
		]
	];
}

FReply SLobbyMenuWidget::OnGarageClicked() const
{
	if (!OwningHUD->GarageWidget)
		OwningHUD->GarageWidget = SNew(SGarageMenuWidget).OwningHUD(OwningHUD);
	
	OwningHUD->MenuWidgetContainer.Get()->SetContent(OwningHUD->GarageWidget.ToSharedRef());
	
	return FReply::Handled();
}

FReply SLobbyMenuWidget::OnVetoMapClicked() const
{
	return FReply::Handled();
}

FReply SLobbyMenuWidget::OnBackClicked() const
{
	// TODO: Add "are you sure" prompt
	
	UPolyRacingSessionSubsystem* SessionSubsystem = OwningHUD->GetGameInstance()->GetSubsystem<UPolyRacingSessionSubsystem>();
	SessionSubsystem->DestroySession();
	
	FString const LevelOptions = FString(TEXT("-game=/Game/GameModes/BP_MainMenuGamemode.BP_MainMenuGamemode_C"));
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), "/Game/Scenes/MainMenuScene", true, LevelOptions);
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
