// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "S08_NetworkingCourse/GenericStructs.h"
#include "LobbyGameState.generated.h"

class UServerStatus;

/**
 * 
 */
UCLASS()
class S08_NETWORKINGCOURSE_API ALobbyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	ALobbyGameState();

	//Functions
public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_PlayerNames();

	void OnPlayerListChanged();

	void TryKickPlayer(FPlayerData PlayerToKick);

public:
	// Array of player data that joined
	UPROPERTY(ReplicatedUsing = OnRep_PlayerNames)
	TArray<FPlayerData> JoinedPlayerStates;

	UServerStatus* ServerStatusWidget;

private:
	TSubclassOf<UUserWidget> ServerStatusBPClass;

};
