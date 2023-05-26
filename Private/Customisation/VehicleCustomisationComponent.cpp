// Fill out your copyright notice in the Description page of Project Settings.


#include "Customisation/VehicleCustomisationComponent.h"


// Sets default values for this component's properties
UVehicleCustomisationComponent::UVehicleCustomisationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UVehicleCustomisationComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UVehicleCustomisationComponent::SetPrimaryColor(UMaterialInstance* DesiredMaterialInstance)
{
	EditingMesh->SetMaterial(1, DesiredMaterialInstance);
}

void UVehicleCustomisationComponent::SetAccentColor(UMaterialInstance* DesiredMaterialInstance)
{
	EditingMesh->SetMaterial(0, DesiredMaterialInstance);
}



