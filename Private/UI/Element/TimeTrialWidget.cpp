// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Element/TimeTrialWidget.h"

#include "Components/TextBlock.h"
#include "Framework/PolyRacingPlayerState.h"


void UTimeTrialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APolyRacingPlayerState>();
}

void UTimeTrialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!PlayerState)
		return;

	double const CurrentTime = GetWorld()->GetTimeSeconds();

	TotalTimeValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(CurrentTime - PlayerState->EventStartTime, 2)));
	CurrentLapTimeValueTextBlock->SetText(FText::FromString(FString::SanitizeFloat(CurrentTime - PlayerState->LastLapStartTime, 2)));
}
