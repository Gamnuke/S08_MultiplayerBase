// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerStatus.h"
#include "Engine/World.h"
#include "S08_NetworkingCourse/Backend/MainGameInstance.h"
#include "S08_NetworkingCourse/Backend/Gamestates/LobbyGameState.h"
#include "Engine/Engine.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/PlayerTab.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/KickedNote.h"


UServerStatus::UServerStatus(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerTab(TEXT("/Game/UI/PlayerTab_WBP"));
	if (PlayerTab.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *PlayerTab.Class->GetName());
		PlayerTabClass = PlayerTab.Class;
	}

}

/*
Binds button click delegates
*/
bool UServerStatus::Initialize() {
	if (!Super::Initialize()) { return false; }

	StartGameButton->OnClicked.AddDynamic(this, &UServerStatus::StartGame);
	return true;
}

/*
Adds widget to player screen and sets input mode
*/
void UServerStatus::Setup() {
	this->AddToPlayerScreen();
	this->SetVisibility(ESlateVisibility::Visible);

	APlayerController *PlayerController = GetOwningPlayer();

	// Settings for cursor and input mode
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(this->TakeWidget());
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;

}

void UServerStatus::TearDown() {
	FInputModeGameOnly InputMode;
	GetOwningPlayer()->bShowMouseCursor = false;
	GetOwningPlayer()->SetInputMode(InputMode);
	this->RemoveFromParent();
}

/*
Moves everyone to the game level when Start Game button is pressed
*/
void UServerStatus::StartGame() {
	if (GetWorld() != nullptr) {
		UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
		if (GameInstance != nullptr) {
			GameInstance->UpdateSessionSettings();
		}
		TearDown();
		GetWorld()->ServerTravel("/Game/Levels/GameMap?listen");
	}
}

/*
Called when player list in lobby game state is changed
*/
void UServerStatus::OnPlayerListChanged() {

	VotekickMenu->UpdatePlayerTabs();
}

