#include "Customisation/VehicleCustomiser.h"

#include "DataTables.h"
#include "Engine/DataTable.h"


TSharedPtr<FVehicleCustomiser> FVehicleCustomiser::Instance = nullptr;

void FVehicleCustomiser::Initialize()
{
	if (!Instance.IsValid())
	{
		Instance = Create();
	}
}

void FVehicleCustomiser::Shutdown()
{
	ensure(Instance.IsUnique()); 
	Instance.Reset();
}

TSharedRef<FVehicleCustomiser> FVehicleCustomiser::Create()
{
	TSharedRef<FVehicleCustomiser> NewCustomiser = MakeShareable(new FVehicleCustomiser());

	// Import vehicle customisation data tables
	// TODO: Find way to load DataTable when not in constructor
	TSoftObjectPtr<UDataTable> const VehicleTypeData = TSoftObjectPtr<UDataTable>(FSoftObjectPath(TEXT("/Game/VehicleCustomisation/VehicleTypes.VehicleTypes")));
	if (VehicleTypeData) NewCustomiser->VehicleTypes = VehicleTypeData.LoadSynchronous();
	else UE_LOG(LogTemp, Warning, TEXT("VehicleCustomiser: No vehicle type data table found!"));
	
	return NewCustomiser;
}

void FVehicleCustomiser::SetupVehicle()
{
	if (!VehicleTypes)
	return;

	const FVehicleType* DefaultBody = VehicleTypes->FindRow<FVehicleType>(FName("Exotic"), "");
	
	UE_LOG(LogTemp, Warning, TEXT("###### FVehicleCustomiser Start ######"));
	UE_LOG(LogTemp, Warning, TEXT("MyFunction called with value: %s"), *DefaultBody->Mesh.ToString());
	PreviewVehicle->BodyMesh->SetSkeletalMesh(DefaultBody->Mesh.LoadSynchronous());
	
	// PreviewVehicle->WheelFrontLeft->SetRelativeLocation(FVector(-DefaultBody->AxelLength, DefaultBody->FrontAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontRight->SetRelativeLocation(FVector(DefaultBody->AxelLength, DefaultBody->FrontAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontLeft->SetRelativeLocation(FVector(-DefaultBody->AxelLength, DefaultBody->RearAxelOffset, 0.f));
	// PreviewVehicle->WheelFrontRight->SetRelativeLocation(FVector(DefaultBody->AxelLength, DefaultBody->RearAxelOffset, 0.f));
}

TSharedPtr<FVehicleCustomiser> FVehicleCustomiser::Get()
{
	return Instance;
}
