// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "DSGameMode.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

ADSGameMode* ABaseCharacter::GetDSGameMode()
{
	if (dsMode == nullptr) {
		dsMode = Cast<ADSGameMode>(UGameplayStatics::GetGameMode(this));
	}
	
	return dsMode;
}

void ABaseCharacter::StartBattle()
{
	GetDSGameMode()->StartBattle();
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

