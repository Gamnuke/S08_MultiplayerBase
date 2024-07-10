// Fill out your copyright notice in the Description page of Project Settings.

#include "MainGameInstance.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "S08_NetworkingCourse/MenuSystem/Menu/MainMenu.h"
#include "S08_NetworkingCourse/MenuSystem/InGameUI/PauseMenu.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/SessionTab.h"
#include "S08_NetworkingCourse/MenuSystem/Menu/ServerStatus.h"
#include "S08_NetworkingCourse/MenuSystem/InGameUI/InGameHUD.h"
#include "S08_NetworkingCourse/Player/GamePlayerController.h"

#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

#include "GameFramework/OnlineSession.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameSession.h"
#include "GameFramework/PlayerController.h"


const static FName SESSION_NAME = NAME_GameSession;
const static FName SESSION_SETTINGS_KEY = TEXT("ServerName");
const static FName SESSION_SETTINGS_Locked = TEXT("ServerLocked");

/**
Ideally the Gamemode handles all the server functions, and gets notified when a new player joins, whether the server host or client
The gamemode then calls things on the PlayerController or Gamestate which can get the game instance running on the connected player's client,
which can be used to do client side stuff for UI for example.
**/

UMainGameInstance::UMainGameInstance(const FObjectInitializer& ObjectInitializer)
{

	static ConstructorHelpers::FClassFinder<UUserWidget> UserWidget(TEXT("/Game/UI/MainMenu_WBP"));
	if (UserWidget.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *UserWidget.Class->GetName());
		MenuClass = UserWidget.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenu(TEXT("/Game/UI/InGame/PauseMenu_WBP"));
	if (PauseMenu.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *PauseMenu.Class->GetName());
		PauseMenuClass = PauseMenu.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameHUD(TEXT("/Game/UI/InGame/InGameHUD_WBP"));
	if (InGameHUD.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *InGameHUD.Class->GetName());
		InGameHUDClass = InGameHUD.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ServerStatus(TEXT("/Game/UI/Lobby/ServerStatus_WBP"));
	if (ServerStatus.Class != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ServerStatus.Class->GetName());
		ServerStatusClass = ServerStatus.Class;
	}
}



void UMainGameInstance::Init() {
	IOnlineSubsystem *Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem: %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMainGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMainGameInstance::OnJoinSessionComplete);
			FindServers();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No subsystem found"));
	}

	GetEngine()->OnNetworkFailure().AddUObject(this, &UMainGameInstance::HandleNetworkFailure);
	GetEngine()->OnTravelFailure().AddUObject(this, &UMainGameInstance::HandleTravelFailure);
}

/*
Handles network errors
*/
void UMainGameInstance::HandleNetworkFailure(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString) {

	AddDebugMessage("Handling error");

	switch (FailureType)
	{
	case ENetworkFailure::NetDriverAlreadyExists:
		break;
	case ENetworkFailure::NetDriverCreateFailure:
		break;
	case ENetworkFailure::NetDriverListenFailure:
		break;
	case ENetworkFailure::ConnectionLost:
		TravelToMainMenu();
		break;
	case ENetworkFailure::ConnectionTimeout:
		TravelToMainMenu();
		break;
	case ENetworkFailure::FailureReceived:
		break;
	case ENetworkFailure::OutdatedClient:
		TravelToMainMenu();
		break;
	case ENetworkFailure::OutdatedServer:
		TravelToMainMenu();
		break;
	case ENetworkFailure::PendingConnectionFailure:
		TravelToMainMenu();
		break;
	case ENetworkFailure::NetGuidMismatch:
		break;
	case ENetworkFailure::NetChecksumMismatch:
		break;
	default:
		break;
	}
}

/*
Handles failiures when travelling
*/
void UMainGameInstance::HandleTravelFailure(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString
)
{
	AddDebugMessage("Handling travel failiure");
	TravelToMainMenu();

}

//------UI------//

/*
Creates and loads the MainMenu widget
*/
void UMainGameInstance::LoadMenuWidget(bool bAddToScreen) {

	// Menu already exists, so return
	if (MenuWidget == nullptr) { 
		MenuWidget = CreateWidget<UMainMenu>(this, MenuClass, FName("Menu Widget"));
		MenuWidget->Setup();
		MenuWidget->SetMenuInterface(this);
	}

	if (bAddToScreen) {
		MenuWidget->SetVisibility(ESlateVisibility::Visible);
		MenuWidget->AddToViewport();
	}
}

/*
Creates and loads the Server Status widget
*/
void UMainGameInstance::LoadServerStatusWidget(bool bAddToScreen) {

	// ServerStatusWidget already exists, so return
	if (ServerStatusWidget == nullptr) {
		ServerStatusWidget = CreateWidget<UServerStatus>(this, ServerStatusClass, FName("ServerStatus Widget"));
	}

	if (bAddToScreen) {
		ServerStatusWidget->SetVisibility(ESlateVisibility::Visible);
		ServerStatusWidget->AddToViewport();
		ServerStatusWidget->Setup();
	}
}

/*
Takes memory address to widget and destroys it if it exists.
*/
void UMainGameInstance::UnloadWidget(UUserWidget *WidgetToUnload, bool bDestroy) {

	// Check if widget exists
	if (WidgetToUnload == nullptr) { return; }
	WidgetToUnload->RemoveFromParent();
	
	if (bDestroy) { WidgetToUnload->Destruct(); }
}



void UMainGameInstance::SetupGame() {
	if (MenuWidget != nullptr) {
		MenuWidget->TearDown(NULL);
	}

	if (PauseMenuWidget != nullptr) {
		PauseMenuWidget->Teardown();
	}
}

void UMainGameInstance::JoinServer(int Index) {
	if (!SessionSearch.IsValid()) { return; }
	if (!SessionInterface.IsValid()) { return; }
	if (!SessionSearch->SearchResults.IsValidIndex(Index)) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Joining server"));
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMainGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid()) { return; }
	FString Address; // Reference to the string of address
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Couldnt get connect string"));
		return;
	}
	if (GetWorld() == nullptr) { return; }

	APlayerController *PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (PlayerController == nullptr) { return; }

	if (MenuWidget != nullptr) {
		MenuWidget->TearDown(false);
	}
	MenuWidget->PlayerIsHost = false;
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMainGameInstance::HostServer(FText ServerName) {
	DesiredServerName = ServerName.ToString();
	if (SessionInterface.IsValid()) {
		auto *ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if(ExistingSession != nullptr){
			GetEngine()->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Orange, FString(ExistingSession->SessionName.ToString().Append(TEXT("- Found existing session, destroying..."))));
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else 
		{
			CreateSession();
		}
	}
}

void UMainGameInstance::CreateSession() {
	if (SessionInterface.IsValid()) {
		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") {
			Settings.bIsLANMatch = true;
		}
		else {
			Settings.bIsLANMatch = false;
		}

		Settings.bShouldAdvertise = true;
		if (MenuWidget != nullptr && MenuWidget->PlayerCounter != nullptr) {
			if (MenuWidget->PlayerCounter->GetText().ToString().IsNumeric()) {
				Settings.NumPublicConnections = FCString::Atoi(*MenuWidget->PlayerCounter->GetText().ToString());
			}
		}

		Settings.Set(SESSION_SETTINGS_Locked, false, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		Settings.bUsesPresence = true;
		UE_LOG(LogTemp, Warning, TEXT("Attempting to host server"));
		Settings.Set(SESSION_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
		SessionInterface->CreateSession(0, SESSION_NAME, Settings);
	}
}

void UMainGameInstance::UpdateSessionSettings() {
	if (SessionInterface.IsValid()) {
		if (MenuWidget != nullptr && MenuWidget->LockSessionBox != nullptr) {
			Settings.Set(SESSION_SETTINGS_Locked, (MenuWidget->LockSessionBox->GetCheckedState() == ECheckBoxState::Checked), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
			SessionInterface.Get()->UpdateSession(SESSION_NAME, Settings, true);
		}
	}
}

void UMainGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		GetEngine()->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Red, FString(SessionName.ToString().Append(TEXT("- Failed to create session"))));
		return;
	}
	GetEngine()->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString(SessionName.ToString().Append(TEXT("- Successfully created session"))));
	MenuWidget->PlayerIsHost = true;
	MenuWidget->TearDown(true);
	GetEngine()->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString(TEXT("Hosting Server")));
	GetWorld()->ServerTravel("/Game/Levels/Lobby?listen");
}

void UMainGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	GetEngine()->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, FString(SessionName.ToString().Append(TEXT("- Successfully destroyed session"))));
	CreateSession();
}

void UMainGameInstance::FindServers() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionSearch->MaxSearchResults = 100;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		FindSessionStartTime = GetWorld()->GetTimeSeconds();
	}
}
void UMainGameInstance::OnFindSessionComplete(bool Success) {
	UE_LOG(LogTemp, Warning, TEXT("Finished Session Search, time taken: %f"), (-FindSessionStartTime + GetWorld()->GetTimeSeconds()));

	if (Success && SessionSearch.IsValid() && MenuWidget != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Sessions found: %d"), SessionSearch->SearchResults.Num());
		MenuWidget->UpdateSessionTabs(SessionSearch);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
	}
}

void UMainGameInstance::OpenPauseMenu() {
	if (PauseMenuWidget == nullptr && PauseMenuClass != nullptr) {
		PauseMenuWidget = CreateWidget<UPauseMenu>(this, PauseMenuClass, FName("Pause Menu Widget"));
	}
	if (PauseMenuWidget != nullptr) {
		PauseMenuWidget->SetMenuInterface(this);
		PauseMenuWidget->IsHost = MenuWidget->PlayerIsHost;
		PauseMenuWidget->Setup();
	}
}

/*
Travels the client to the main menu
*/
void UMainGameInstance::TravelToMainMenu() {
	APlayerController *PlayerController = GetFirstLocalPlayerController();
	if (PlayerController != nullptr) {
		if (PauseMenuWidget != nullptr) {
			PauseMenuWidget->Teardown();
		}
		PlayerController->ClientTravel("/Game/Levels/MainMenu", ETravelType::TRAVEL_Absolute, true);
	}
}

void UMainGameInstance::OpenServerStatusMenu() {
	if (ServerStatusWidget == nullptr && ServerStatusClass != nullptr) {
		ServerStatusWidget = CreateWidget<UServerStatus>(this, ServerStatusClass, FName("ServerStatusMenu"));
	}
	if (ServerStatusWidget != nullptr) {
		ServerStatusWidget->Setup();
	}
}

void UMainGameInstance::CreateTab() {
	if (MenuWidget != nullptr) {
		MenuWidget->CreateTab();
	}
}

void UMainGameInstance::SetServerName(FString NewServerName) {
	DesiredServerName = NewServerName;
}

void UMainGameInstance::UpdatePlayerTabs() {
	if (ServerStatusWidget == nullptr) { return; }
}

void UMainGameInstance::AddDebugMessage(FString NewMessage)
{
	if (GEngine != nullptr) {
		GEngine->AddOnScreenDebugMessage(FMath::Rand(), 3, FColor::Green, NewMessage);
	}
}





