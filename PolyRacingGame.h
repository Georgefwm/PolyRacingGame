// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ui/UiStyles.h"
#include "Styling/SlateStyleRegistry.h"

class FPolyRacingGame : public FDefaultGameModuleImpl
{
	// Called whenever the module is starting up. In here, we unregister any style sets 
	// (which may have been added by other modules) sharing our 
	// style set's name, then initialize our style set. 
	virtual void StartupModule() override
	{
		//Hot reload hack
		FSlateStyleRegistry::UnRegisterSlateStyle(FUiStyles::GetStyleSetName());
		FUiStyles::Initialize();
	}

	// Called whenever the module is shutting down. Here, we simply tell our MenuStyles to shut down.
	virtual void ShutdownModule() override
	{
		FUiStyles::Shutdown();
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE(FPolyRacingGame, PolyRacing, "PolyRacing");