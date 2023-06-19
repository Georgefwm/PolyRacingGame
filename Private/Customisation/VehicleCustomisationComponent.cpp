// Fill out your copyright notice in the Description page of Project Settings.


#include "Customisation/VehicleCustomisationComponent.h"

#include "PolyRacingWheeledVehiclePawn.h"
#include "Customisation/VehicleCustomiser.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UVehicleCustomisationComponent::UVehicleCustomisationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
}

void UVehicleCustomisationComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UVehicleCustomisationComponent, EditingMesh);
	DOREPLIFETIME(UVehicleCustomisationComponent, CurrentPrimaryColor);
	DOREPLIFETIME(UVehicleCustomisationComponent, CurrentAccentColor);
}

void UVehicleCustomisationComponent::OnRep_PrimaryColorChanged()
{
	UVehicleCustomiser* VehicleCustomiser = GetWorld()->GetGameInstance()->GetSubsystem<UVehicleCustomiser>();

	if (!VehicleCustomiser)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep called but vehicle customiser isnt valid"))
		return;
	}

	if (APolyRacingWheeledVehiclePawn* Vehicle = Cast<APolyRacingWheeledVehiclePawn>(GetOwner()))
		VehicleCustomiser->SetPrimaryColor(Vehicle, CurrentPrimaryColor);
}

void UVehicleCustomisationComponent::OnRep_AccentColorChanged()
{
	UVehicleCustomiser* VehicleCustomiser = GetWorld()->GetGameInstance()->GetSubsystem<UVehicleCustomiser>();

	if (!VehicleCustomiser)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnRep called but vehicle customiser isnt valid"))
		return;
	}

	if (APolyRacingWheeledVehiclePawn* Vehicle = Cast<APolyRacingWheeledVehiclePawn>(GetOwner()))
		VehicleCustomiser->SetAccentColor(Vehicle, CurrentAccentColor);
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



