// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/MainMenuGameMode.h"

#include "Camera/CameraActor.h"
#include "UI/MenuHUD.h"
#include "Controller/MenuPlayerController.h"
#include "Customisation/VehicleCustomiser.h"
#include "Kismet/GameplayStatics.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PlayerControllerClass = AMenuPlayerController::StaticClass();
	HUDClass = AMenuHUD::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Setup the menu background vehicle
	UVehicleCustomiser* VehicleCustomiser = GetGameInstance()->GetSubsystem<UVehicleCustomiser>();
	VehicleCustomiser->SetupVehicle();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

	if (!FoundActors.IsEmpty())
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(FoundActors[0], 0.0f, EViewTargetBlendFunction::VTBlend_Linear);
	
}
