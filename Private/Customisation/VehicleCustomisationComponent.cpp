// Fill out your copyright notice in the Description page of Project Settings.


#include "Customisation/VehicleCustomisationComponent.h"


// Sets default values for this component's properties
UVehicleCustomisationComponent::UVehicleCustomisationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UVehicleCustomisationComponent::BeginPlay()
{
	Super::BeginPlay();

	EditingMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}

void UVehicleCustomisationComponent::AttachMesh(FName SocketName, TSoftObjectPtr<USkeletalMesh> SkeletalMesh)
{
	
	//m_cUClassWeapon = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/CharacterController/BP_Pistol.BP_Pistol_C"));
	FActorSpawnParameters SpawnParams = FActorSpawnParameters();

	
	
	//SkeletalMesh.Tr->AttachRootComponentTo(EditingMesh, SocketName, EAttachLocation::SnapToTarget, true);
}



