// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePreview.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AVehiclePreview::AVehiclePreview()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("VehicleSceneRoot");
	SetRootComponent(SceneRoot);

	// SetRootComponent(DefaultSceneRoot);
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleBody");
    BodyMesh->SetupAttachment(SceneRoot);

    BonnetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleBonnet");
    BonnetMesh->SetupAttachment(BodyMesh);
    
    BumperFrontMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleBumperFront");
    BumperFrontMesh->SetupAttachment(BodyMesh);

    BumperRearMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleBumperRear");
    BumperRearMesh->SetupAttachment(BodyMesh);

    SideSkirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleSideSkirt");
    SideSkirtMesh->SetupAttachment(BodyMesh);

    FenderMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleFender");
    FenderMesh->SetupAttachment(BodyMesh);
    
    UndercarriageMesh = CreateDefaultSubobject<USkeletalMeshComponent>("VehicleUndercarriage");
    UndercarriageMesh->SetupAttachment(BodyMesh);
}

// Called when the game starts or when spawned
void AVehiclePreview::BeginPlay()
{
	Super::BeginPlay();
	//USlateStyleSubsystem* MySubsystem = UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<USlateStyleSubsystem>();
}

// Called every frame
void AVehiclePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

