// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePreview.h"

#include "DataTables.h"

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

	// Wheels
    
	FrontLeftRim = CreateDefaultSubobject<USkeletalMeshComponent>("FrontLeftRim");
	FrontLeftRim->SetupAttachment(UndercarriageMesh);

	FrontRightRim = CreateDefaultSubobject<USkeletalMeshComponent>("FrontRightRim");
	FrontRightRim->SetupAttachment(UndercarriageMesh);

	RearLeftRim = CreateDefaultSubobject<USkeletalMeshComponent>("RearLeftRim");
	RearLeftRim->SetupAttachment(UndercarriageMesh);

	RearRightRim = CreateDefaultSubobject<USkeletalMeshComponent>("RearRightRim");
	RearRightRim->SetupAttachment(UndercarriageMesh);

	FrontLeftTyre = CreateDefaultSubobject<USkeletalMeshComponent>("FrontLeftTyre");
	FrontLeftTyre->SetupAttachment(FrontLeftRim);

	FrontRightTyre = CreateDefaultSubobject<USkeletalMeshComponent>("FrontRightTyre");
	FrontRightTyre->SetupAttachment(FrontRightRim);

	RearLeftTyre = CreateDefaultSubobject<USkeletalMeshComponent>("RearLeftTyre");
	RearLeftTyre->SetupAttachment(RearLeftRim);

	RearRightTyre = CreateDefaultSubobject<USkeletalMeshComponent>("RearRightTyre");
	RearRightTyre->SetupAttachment(RearRightRim);

	// TODO: Setup rest of customisable parts
}

// Called when the game starts or when spawned
void AVehiclePreview::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVehiclePreview::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehiclePreview::SetWheelOffsets(FVehicleType* VehicleType)
{
	FVector const WheelScalar = FVector(VehicleType->WheelSizeModifier, VehicleType->WheelSizeModifier, VehicleType->WheelSizeModifier);
	
	FrontLeftRim->SetRelativeLocation(FVector(VehicleType->FrontAxelOffset, -VehicleType->AxelLength, 40.f));
	FrontLeftRim->SetRelativeScale3D(-WheelScalar);
	
	FrontRightRim->SetRelativeLocation(FVector(VehicleType->FrontAxelOffset, VehicleType->AxelLength, 40.f));
	FrontRightRim->SetRelativeScale3D(WheelScalar);

	RearLeftRim->SetRelativeLocation(FVector(VehicleType->RearAxelOffset, -VehicleType->AxelLength, 40.f));
	RearLeftRim->SetRelativeScale3D(-WheelScalar);
	
	RearRightRim->SetRelativeLocation(FVector(VehicleType->RearAxelOffset, VehicleType->AxelLength, 40.f));
	RearRightRim->SetRelativeScale3D(WheelScalar);
}

