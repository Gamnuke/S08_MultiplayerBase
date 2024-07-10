// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class APlayerController;
/**
 * 
 */
UCLASS()
class S08_NETWORKINGCOURSE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	void PostLogin(APlayerController* NewPlayer);

};
