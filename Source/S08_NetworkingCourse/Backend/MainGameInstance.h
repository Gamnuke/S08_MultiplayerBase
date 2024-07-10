// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/MenuInterface.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

#include "MainGameInstance.generated.h"

class UMainMenu;
class UServerStatus;
class USessionTab;
class UPauseMenu;
class UInGameHUD;
/**
 * 
 */


UCLASS()
class S08_NETWORKINGCOURSE_API UMainGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UMainGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init();

	UFUNCTION()
	void HandleNetworkFailure(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	UFUNCTION()
	void HandleTravelFailure(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString);

	void LoadMenuWidget(bool bAddToScreen = false);

	void LoadServerStatusWidget(bool bAddToScreen = false);

	void UpdatePlayerTabs();

	UFUNCTION(exec)
		void JoinServer(int Index);
	
	UFUNCTION(exec)
		void HostServer(FText ServerName);

	UFUNCTION(exec)
		void FindServers();

	UFUNCTION(exec)
		void TravelToMainMenu();

	UFUNCTION(BlueprintCallable)
		void OpenPauseMenu();

	UFUNCTION(exec)
		void OpenServerStatusMenu();

	void UpdateSessionSettings();

	UPROPERTY()
		UMainMenu *MenuWidget;
	UPROPERTY()
		UPauseMenu *PauseMenuWidget;
	UPROPERTY()
		UServerStatus *ServerStatusWidget;
	UPROPERTY()
		UInGameHUD *InGameHUDWidget;

		void UnloadWidget(UUserWidget* WidgetToUnload, bool bDestroy);

		UFUNCTION(BlueprintCallable)
	void SetupGame();

	UFUNCTION(exec)
		void CreateTab();


	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
	void SetServerName(FString NewServerName);
	static void AddDebugMessage(FString NewMessage);

	float FindSessionStartTime;
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> PauseMenuClass;
	TSubclassOf<class UUserWidget> ServerStatusClass;
	TSubclassOf<class UUserWidget> InGameHUDClass;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	FString DesiredServerName;

	FOnlineSessionSettings Settings;
};
