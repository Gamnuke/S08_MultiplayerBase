// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/MainMenu.h"
#include "S08_NetworkingCourse/Player/GamePlayerController.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/Backend/Gamestates/LobbyGameState.h"

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer) {
	GameStateClass = ALobbyGameState::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{

	Super::PostLogin(NewPlayer);

	// Create new playerdata to store in gamestate
	FPlayerData NewData;
	NewData.PlayerID = NewPlayer->PlayerState->GetPlayerId();
	NewData.PlayerName = NewPlayer->PlayerState->GetPlayerName();
	NewData.PlayerState = NewPlayer->PlayerState;
	GetLobbyGameState()->JoinedPlayerStates.Add(NewData);

	// Load server status widget for new player
	AGamePlayerController* JoinedPlayer = Cast<AGamePlayerController>(NewPlayer);

	// Can't call LoadServerStatusWidget directly on JoinedPlayer as the game instance will point to the server's game instance
	// and wont load the UI, so instead call it on the player controller as a client RPC 
	JoinedPlayer->LoadServerStatusWidget();

	// Notify that player list is changed
	GetLobbyGameState()->OnPlayerListChanged();

	//NumOfPlayers += 1;
	//Validate();
	//bUseSeamlessTravel = true;
	//UE_LOG(LogTemp, Warning, TEXT("A player has joined the lobby."));
	////if (GetNetOwningPlayer()->PlayerController->NetPlayerIndex == 0) {
	//	if (GetGameInstance() == nullptr) { return; }
	//	UMainGameInstance *GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	//	if (GameInstance != nullptr) {
	//		GameInstance->GetEngine()->AddOnScreenDebugMessage(0, 1000, FColor::Cyan, FString("A Player Has Logged In!"));
	//	}
	//	if (GameInstance != nullptr && GameInstance->MenuWidget != nullptr) {
	//		GameInstance->MenuWidget->Setup();
	//		if (GameInstance->MenuWidget->PlayerIsHost) {
	//			GameInstance->MenuWidget->ToServerStatus_Host();
	//		}
	//		else {
	//			GameInstance->MenuWidget->ToServerStatus_Client();
	//		}
	//	}
	////}
}

/*
Gets game state casted to lobby game state
*/
ALobbyGameState* ALobbyGameMode::GetLobbyGameState() {
	return GetGameState<ALobbyGameState>();
}

/*
Deletes player data from array
*/
void ALobbyGameMode::Logout(AController * ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	// Finds the player in the joinedplayers array and deletes them if the player states match.
	for (int i = 0; i < GetLobbyGameState()->JoinedPlayerStates.Num(); i++)
	{
		FPlayerData Player = GetLobbyGameState()->JoinedPlayerStates[i];
		if (Player.PlayerState == ExitingPlayer->PlayerState) {
			GetLobbyGameState()->JoinedPlayerStates.RemoveAt(i);
			break;
		}
	}

	GetLobbyGameState()->OnPlayerListChanged();
}

void ALobbyGameMode::Validate() {
	/*if (NumOfPlayers >= MaxPlayers) {
		UE_LOG(LogTemp, Warning, TEXT("Server full"));

		UWorld *World = GetWorld();
		if (World != nullptr) {
			World->ServerTravel("/Game/Levels/GameMap?listen");
		}
	}*/
}


