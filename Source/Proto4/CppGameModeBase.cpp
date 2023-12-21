// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "BasePawn.h"
#include "TurretPawn.h"

void ACppGameModeBase::ActorDied(AActor *DeadActor)
{
    if (DeadActor == BasePawn)
    {
        BasePawn->HandleDestruction();
        if (BasePawn->GetTurretPlayerController())
        {
            BasePawn->DisableInput(BasePawn->GetTurretPlayerController());
            BasePawn->GetTurretPlayerController()->bShowMouseCursor = false;
        }
    }
    else if (ATurretPawn* DestroyedTurret = Cast<ATurretPawn>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
    }
}

void ACppGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    BasePawn = Cast<ABasePawn>(UGameplayStatics::GetPlayerPawn(this, 0));
}

