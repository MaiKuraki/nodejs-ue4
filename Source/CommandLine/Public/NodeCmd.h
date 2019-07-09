#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Async/Async.h"
#include "SocketIONative.h"
#include "Runtime/Core/Public/HAL/ThreadSafeBool.h"

class COMMANDLINE_API FNodeEventListener
{
public:
	TFunction<void(const FString& LogMsg)> OnConsoleLog;
	TFunction<void(const FString& LogMsg)> OnScriptConsoleLog;
	TFunction<void(const FString& ScriptRelativePath)> OnMainScriptEnd;
	TFunction<void(int32 ProcessId)> OnChildScriptBegin;
	TFunction<void(int32 ScriptId)> OnChildScriptEnd;
	TFunction<void(const FString& ScriptRelativePath, const FString& ScriptErrorMessage)> OnScriptError;
	int32 ProcessId;

	FNodeEventListener() 
	{
		OnConsoleLog = nullptr;
		OnScriptConsoleLog = nullptr;
		OnMainScriptEnd = nullptr;
		OnChildScriptBegin = nullptr;
		OnChildScriptEnd = nullptr;
		OnScriptError = nullptr;
		ProcessId = -1;
	}
};

class COMMANDLINE_API FNodeCmd
{
public:
	FNodeCmd();
	~FNodeCmd();
	
	//Start a node.js script
	void RunChildScript(const FString& ScriptRelativePath);

	//Forcefully stop the script
	void StopMainScript();
	void StopChildScript(int32 ProcessId);

	bool IsMainScriptRunning();

	//listening to events node.js
	void AddEventListener(TSharedPtr<FNodeEventListener> Listener);
	void RemoveEventListener(TSharedPtr<FNodeEventListener> Listener);

	TSharedPtr<FSocketIONative> Socket;

	FString DefaultMainScript;
	int32 DefaultPort;
	bool bShouldStopMainScriptOnNoListeners;
	bool bUseRemoteMainScript;

private:
	void StartupMainScriptIfNeeded();
	void StopMainScriptSync();

	//start wrapper script
	bool RunMainScript(FString ScriptRelativePath, int32 Port = 3000);

	TArray<TSharedPtr<FNodeEventListener>> Listeners;
	TArray<int32>RunningChildScripts;

	FString ProcessDirectory;
	FString PluginContentRelativePath;

	FThreadSafeBool bShouldMainRun;
	FThreadSafeBool bIsMainRunning;
};