// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayingGameMode.h"
#include "LobbyGameMode.generated.h"

class AGamePlayerController;
class ALobbyGameState;



/**
 * 
 */
UCLASS()
class S08_NETWORKINGCOURSE_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ALobbyGameMode(const FObjectInitializer& ObjectInitializer);

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController * ExitingPlayer) override;

private:
	int NumOfPlayers;

public:
	int MaxPlayers = 3;
	void Validate();

private:
	ALobbyGameState* GameState;

	ALobbyGameState* GetLobbyGameState();
};
