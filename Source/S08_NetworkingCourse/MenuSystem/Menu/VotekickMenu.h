// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "S08_NetworkingCourse/GenericStructs.h"
#include "VotekickMenu.generated.h"

class UPlayerTab;
struct FPlayerData;

UCLASS()
class S08_NETWORKINGCOURSE_API UVotekickMenu : public UUserWidget
{
	GENERATED_BODY()

	UVotekickMenu(const FObjectInitializer& ObjectInitializer);
protected:
	virtual bool Initialize();
		/// Vote kick menu
private:
	TSubclassOf<class UUserWidget> PlayerTabClass;
	TSubclassOf<class UUserWidget> VotekickMenuClassBlueprint;
	UPlayerTab *SelectedTab;

	UPROPERTY(meta = (BindWidget))
	class UButton *VoteKickButton;

	UPROPERTY(meta = (BindWidget))
	class UButton *RefreshButton;

	UPROPERTY(meta = (BindWidget))
	class UWidget *VoteKickMenu;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox *PlayerBox;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox *ReasonBox;

	UFUNCTION()
	void CallVoteKick();

public:
	void SetSelectedPlayerIndex(FPlayerData* Index, UPlayerTab * OwningTab);
	TArray<UPlayerTab*> Tabs;
	FPlayerData* SelectedIndex = nullptr;

	UFUNCTION()
		void UpdatePlayerTabs();
};
