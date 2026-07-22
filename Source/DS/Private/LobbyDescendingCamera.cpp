

#include "LobbyDescendingCamera.h"
#include "Camera/CameraComponent.h"

ALobbyDescendingCamera::ALobbyDescendingCamera()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	RootComponent = Camera;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ALobbyDescendingCamera::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void ALobbyDescendingCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= DescendSpeed * DeltaTime;

	if (LoopDepth > 0.f && (StartLocation.Z - NewLocation.Z) >= LoopDepth)
	{
		NewLocation.Z = StartLocation.Z;
	}

	SetActorLocation(NewLocation);
}
