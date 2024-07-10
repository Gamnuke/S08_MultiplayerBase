// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/MainMenu.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
class UInGameHUD;
class UKickedNote;

UCLASS()
class S08_NETWORKINGCOURSE_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UMainMenu *MainMenuWidget;

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Setup_Client();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UInGameHUD *InGameHUDReference;

	void AddWidgetToViewport(UKickedNote *WidgetToAdd);

	UFUNCTION(Client, Reliable)
	void LoadServerStatusWidget();
};
