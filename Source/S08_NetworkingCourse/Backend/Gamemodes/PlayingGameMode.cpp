// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayingGameMode.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "S08_NetworkingCourse/Player/GamePlayerController.h"


APlayingGameMode::APlayingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/MainCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void APlayingGameMode::HandleStartingNewPlayer_Implementation(APlayerController * PlayerController)
{
	Super::HandleStartingNewPlayer_Implementation(PlayerController);
}

