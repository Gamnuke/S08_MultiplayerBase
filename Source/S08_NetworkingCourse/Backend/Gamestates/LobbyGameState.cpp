// Fill out your copyright notice in the Description page of Project Settings.


#include "S08_NetworkingCourse/Backend/Gamestates/LobbyGameState.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/Player/GamePlayerController.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/ServerStatus.h"
#include "S08_NetworkingCourse/GenericStructs.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState() {
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Game/UI/Lobby/ServerStatus_WBP"));
	if (WidgetClass.Class != NULL) { ServerStatusBPClass = WidgetClass.Class; }
	bReplicates = true;
}

void ALobbyGameState::BeginPlay() {
	Super::BeginPlay();

	// Call to update player list once lobby game state is created, turns out player controller gets created before game state is replicated/created
	// so accessing it causes to crash
	UMainGameInstance::AddDebugMessage("Lobby Game State begin play");
	OnPlayerListChanged();
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGameState, JoinedPlayerStates);
}

// Called on all clients except server when joined players variable is changed
void ALobbyGameState::OnRep_PlayerNames()
{
	UMainGameInstance::AddDebugMessage("Replication of player names notified");
	OnPlayerListChanged();
}

// Notify server status widget to update its player list according to player list variable
void ALobbyGameState::OnPlayerListChanged()
{
	UMainGameInstance* GameInstance = GetGameInstance<UMainGameInstance>();
	GameInstance->LoadServerStatusWidget(false);

	if (GameInstance->ServerStatusWidget != nullptr) {
		GameInstance->ServerStatusWidget->OnPlayerListChanged();
	}
}

/*
Try and find the matching player ID and kick the player
*/
void ALobbyGameState::TryKickPlayer(FPlayerData PlayerToKick)
{
	// Since the PlayerState variable in FPlayerData isn't replicated, only hosts can call the kick as they have the PlayerState variable not null
	for (FPlayerData PlayerData : JoinedPlayerStates) {
		if (
			PlayerData.PlayerID == PlayerToKick.PlayerID &&
			PlayerData.PlayerState != nullptr
		) 
		{
			PlayerData.PlayerState->GetPlayerController()->ClientTravel("/Game/Levels/MainMenu", ETravelType::TRAVEL_Absolute);
			OnPlayerListChanged();
			break;
		}
	}
}


