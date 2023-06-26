// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolyRacingStaticUtils.h"


static FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10,-1 * Precision))
	{       
		TheFloat = Rounded;
	}
	
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	
	return FText::AsNumber(TheFloat, &NumberFormat).ToString(); 
}

static FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10,-1 * Precision))
	{       
		TheFloat = Rounded;
	}
	
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	
	return FText::AsNumber(TheFloat, &NumberFormat); 
}