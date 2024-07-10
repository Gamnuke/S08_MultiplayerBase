// Fill out your copyright notice in the Description page of Project Settings.

#include "GamePlayerController.h"
#include "GameFramework/PlayerController.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/KickedNote.h"
#include "Engine/GameInstance.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "GamePlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/GameState.h"
#include "S08_NetworkingCourse/MenuSystem/InGameUI/InGameHUD.h"
#include "S08_NetworkingCourse/Backend/Gamemodes/LobbyGameMode.h"

void AGamePlayerController::Setup_Client_Implementation() {
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

}

void AGamePlayerController::AddWidgetToViewport(UKickedNote *WidgetToAdd) {
	WidgetToAdd->AddToPlayerScreen();
}

/*
Gets the game instance of the client and tells it to construct the server status widget
*/
void AGamePlayerController::LoadServerStatusWidget_Implementation() {
	UMainGameInstance::AddDebugMessage("Creating Server status widget");
	GetGameInstance<UMainGameInstance>()->LoadServerStatusWidget(true);
}

