// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CppGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTO4_API ACppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	void ActorDied(AActor* DeadActor);

protected:

	virtual void BeginPlay() override;

private:

	class ABasePawn* BasePawn;

	
};
