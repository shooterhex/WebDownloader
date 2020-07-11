#pragma once
#include <cstdint>
#include <cassert>
#include <cstring>

#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <set>

#include <string>
#include <codecvt>
#include <cwchar>
#include <locale>

#include <stdexcept>
#include <fstream>
#include <iostream>

#include <functional>
#include <tuple>
#include <limits>

#include <thread>
#include <chrono>

#include <filesystem>
#include <any>

#define HTMLTEXT 100
//framework

// Command
//   [](<parameters>)->bool
//   provider class: std::function<bool(...)> get_XXXCommand();
//   binding class: void attach_XXXCommand(std::function<bool(...)>&& cmd);
//                  std::function<bool(...)> detach_XXXCommand() noexcept;


//   [](std::any&& param)->bool
typedef std::function<bool(std::any&&)>  CommandFunc;


// Property Notification
//   [](uint32_t)
typedef std::function<void(uint32_t)>  PropertyNotification;

class PropertyTrigger
{
public:
    PropertyTrigger() noexcept{};
	PropertyTrigger(const PropertyTrigger&) = delete;
	PropertyTrigger& operator=(const PropertyTrigger&) = delete;
    ~PropertyTrigger() noexcept{};

	void ClearNotifications() noexcept;
    uintptr_t AddNotification(PropertyNotification&& pn);
	void RemoveNotification(uintptr_t uCookie) noexcept;

	void Fire(uint32_t uID);

private:
	std::vector<PropertyNotification> m_vecPN;
};

// state machine

typedef std::function<int32_t(uint32_t)>  StateFunc;
//   provider class: StateFunc get_XXXStateFunc();

class StateManager
{
public:
	StateManager() noexcept;
	StateManager(const StateManager&) = delete;
	StateManager& operator=(const StateManager&) = delete;
	~StateManager() noexcept;

	void Add(int32_t iState, StateFunc&& sf)
	{
		m_map.insert(std::pair<int32_t, StateFunc>(iState, std::move(sf)));
	}
	void SetStartState(int32_t iStartState) noexcept
	{
		m_iCurrentState = iStartState;
	}
	void Process(uint32_t uEvent)
	{
		auto iter(m_map.find(m_iCurrentState));
		if( iter != m_map.end() )
			m_iCurrentState = iter->second(uEvent);
	}
	int32_t GetCurrentState() const noexcept
	{
		return m_iCurrentState;
	}

private:
	int32_t m_iCurrentState;
	std::map<int32_t, StateFunc> m_map;
};
