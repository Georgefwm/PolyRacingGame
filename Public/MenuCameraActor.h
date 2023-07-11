// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MenuCameraActor.generated.h"

class AMenuPlayerController;
class USoundCue;


UCLASS()
class POLYRACINGGAME_API AMenuCameraActor : public ACameraActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMenuCameraActor();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* AudioComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(APlayerController* NewController);
};
