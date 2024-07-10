// Fill out your copyright notice in the Description page of Project Settings.


#include "S08_NetworkingCourse/Backend/Gamemodes/MainMenuGameMode.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/Player/GamePlayerController.h"

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{

	Super::PostLogin(NewPlayer);

	// Once the player has loaded into the main menu level, load the main menu widget
	AGamePlayerController* PlayerController = Cast<AGamePlayerController>(NewPlayer);
	GetGameInstance<UMainGameInstance>()->LoadMenuWidget(true);
}