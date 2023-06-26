// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolyRacingStaticUtils.generated.h"


static FString GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true);

static FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero=true);