// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "S08_NetworkingCourse/MenuSystem/InGameUI/PauseMenu.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/VotekickMenu.h"
#include "S08_NetworkingCourse/GenericStructs.h"
#include "PlayerTab.generated.h"

struct FPlayerData;
/**
 * 
 */
UCLASS()
class S08_NETWORKINGCOURSE_API UPlayerTab : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Function")
	void UpdateColour(bool Active);
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock *PlayerNameBlock;

	UPROPERTY(meta = (BindWidget))
		UButton *Tab;

	void UpdateName(FString Name);

	FPlayerData PlayerID;

	UVotekickMenu *ParentVotekickMenu;

	APlayerState* AssignedPlayerState;

	UFUNCTION()
	void SelectIndex();
};
