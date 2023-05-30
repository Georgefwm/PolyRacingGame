// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "UI/GlobalMenuStyle.h"
#include "UI/MenuHUD.h"
#include "UI/UiStyles.h"

#define LOCTEXT_NAMESPACE "pausemenu"

void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;
	Style = &FUiStyles::Get().GetWidgetStyle<FGlobalStyle>("PolyRacingMenuStyle");

	/** Text */
	const FText ResumeText			= LOCTEXT("ResumeText", "Resume");
	const FText SettingsText		= LOCTEXT("SettingsButtonText", "Settings");
	const FText LeaveText			= LOCTEXT("LeaveText", "Leave");

	
	ChildSlot
		[
			SNew(SOverlay)
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
					.Text(ResumeText)
					.OnClicked(this, &SPauseMenuWidget::OnResumeClicked)
				]
				
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(SettingsText)
					.OnClicked(this, &SPauseMenuWidget::OnSettingsClicked)
				]
				
				+ SVerticalBox::Slot()
				.Padding(Style->MenuButtonSpacingMargin)
				[
					SNew(SButton)
					.ButtonStyle(&Style->MenuButtonStyle)
					.TextStyle(&Style->MenuButtonTextStyle)
					.Text(LeaveText)
					.OnClicked(this, &SPauseMenuWidget::OnLeaveClicked)
				]
			]
		];
}

FReply SPauseMenuWidget::OnResumeClicked() const
{
	OwningHUD->RemoveMenu();
	
	return FReply::Handled();
}

FReply SPauseMenuWidget::OnSettingsClicked() const
{
	UE_LOG(LogTemp, Warning, TEXT("Settings not implemented... yet"));
	
	return FReply::Handled();
}

FReply SPauseMenuWidget::OnLeaveClicked() const
{
	FString const LevelOptions = FString(TEXT("listen -game=/Game/GameModes/BP_MainMenuGamemode.BP_MainMenuGamemode_C"));
	
	UGameplayStatics::OpenLevel(OwningHUD->GetWorld(), "/Game/Scenes/MainMenuScene", true, LevelOptions);
	
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE