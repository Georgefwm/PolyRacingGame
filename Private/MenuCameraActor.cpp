// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuCameraActor.h"
#include "Components/AudioComponent.h"


// Sets default values
AMenuCameraActor::AMenuCameraActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->bIsMusic = true;
	AudioComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMenuCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMenuCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMenuCameraActor::PossessedBy(APlayerController* NewController)
{
	if(!AudioComponent->GetSound())
		return;
	
	AudioComponent->Play();
}

