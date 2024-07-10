// Fill out your copyright notice in the Description page of Project Settings.

#include "VotekickMenu.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/PlayerTab.h"
#include "S08_NetworkingCourse/Backend/Gamestates/LobbyGameState.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/GenericStructs.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UVotekickMenu::UVotekickMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerTab(TEXT("/Game/UI/PlayerTab_WBP"));
	if (PlayerTab.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *PlayerTab.Class->GetName());
		PlayerTabClass = PlayerTab.Class;
	}

}

/*
Delegate buttons to functions
*/
bool UVotekickMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) { return false; }
	//Check if not null

	VoteKickButton->OnClicked.AddDynamic(this, &UVotekickMenu::CallVoteKick);
	RefreshButton->OnClicked.AddDynamic(this, &UVotekickMenu::UpdatePlayerTabs);
	return true;
}

/*
Clear the player tabs, add the update/new player tabs to the votekick list
*/
void UVotekickMenu::UpdatePlayerTabs() {
	// Check if we can get the lobby game state
	ALobbyGameState* LobbyGameState = GetWorld()->GetGameState<ALobbyGameState>();
	UMainGameInstance::AddDebugMessage("Updating player tabs, number of players: " + FString::FromInt(LobbyGameState->JoinedPlayerStates.Num()));

	// Delete all player tabs
	if (PlayerBox != nullptr) {PlayerBox->ClearChildren();}

	Tabs.Empty();
	SelectedIndex = nullptr;

	// Loop through player data variable on lobby game state and create tabs
	if (GetWorld() != nullptr) {
		for (FPlayerData PlayerData : LobbyGameState->JoinedPlayerStates) {
			UPlayerTab *Tab = CreateWidget<UPlayerTab>(this, PlayerTabClass, *(FString("PlayerTab") + PlayerData.PlayerName));
			Tab->UpdateName(PlayerData.PlayerName);
			Tab->ParentVotekickMenu = this;
			Tab->PlayerID = PlayerData;
			Tabs.Add(Tab);
			PlayerBox->AddChild(Tab);
		}
	}
}

/*
Changes color of selected tab
*/
void UVotekickMenu::SetSelectedPlayerIndex(FPlayerData* Index, UPlayerTab *OwningTab) {

	// Unselect other tabs
	for (UPlayerTab *Tab : Tabs) {
		Tab->UpdateColour(false);
	}

	// Select this one
	OwningTab->UpdateColour(true);
	SelectedIndex = Index;
	SelectedTab = OwningTab;
}

/*
Trys to kick player via LobbyGameState
*/
void UVotekickMenu::CallVoteKick() {
	if (SelectedIndex == nullptr) { return; }
	GetWorld()->GetGameState<ALobbyGameState>()->TryKickPlayer(*SelectedIndex);

	/*int* SelectedState = States[SelectedIndex.GetValue()];
	if (SelectedState == nullptr) { return; }
	if (SelectedState->GetNetOwningPlayer() == nullptr) { return; }
	APlayerController *PlayerController = SelectedState->GetNetOwningPlayer()->PlayerController;
	if (PlayerController == nullptr) { return; }
	PlayerController->ClientTravel("/Game/Levels/MainMenu", ETravelType::TRAVEL_Absolute);
	if (SelectedTab != nullptr) {
		SelectedTab->RemoveFromParent();
		SelectedTab = nullptr;
	}*/
	
}


