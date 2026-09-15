/*
game_event_manager.h - class that responsible for registering game events handlers
Copyright (C) 2024 SNMetamorph

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/

#pragma once

class CGameEventManager
{
public:
	CGameEventManager();
	~CGameEventManager() = default;

private:
	CGameEventManager(const CGameEventManager&) = delete;
	CGameEventManager(CGameEventManager&&) = delete;
	CGameEventManager& operator=(const CGameEventManager&) = delete;
	CGameEventManager& operator=(CGameEventManager&&) = delete;

	void RegisterGlockEvents();
	void RegisterCrossbowEvents();
	void RegisterPythonEvents();
	void RegisterMP5Events();
	void RegisterShotgunEvents();
	void RegisterCrowbarEvents();
	void RegisterTripmineEvents();
	void RegisterSnarkEvents();
	void RegisterHornetgunEvents();
	void RegisterRPGEvents();
	void RegisterEgonEvents();
	void RegisterGaussEvents();
	void RegisterPMEvents();
	void RegisterAPSEvents();
	void RegisterGURZAEvents();
	void RegisterGRACHEvents();
	void RegisterOC33Events();
	void RegisterR92Events();
	void RegisterFLAREGUNEvents();
	void RegisterPBEvents();
	void RegisterP96Events();
	void RegisterPSMEvents();
	void RegisterOC23Events();
	void RegisterOC27Events();
	void Register22LREvents();
	void RegisterP7Events();
	void RegisterVP70Events();
	void RegisterKS23Events();
	void RegisterBEKASEvents();
	void RegisterSAIGAEvents();
	void RegisterVEPREvents();
	void RegisterMC255Events();
	void RegisterBIZONEvents();
	void RegisterKEDREvents();
	void RegisterVERESKEvents();
	void RegisterKIPARISEvents();
	void RegisterGEPARDEvents();
	void RegisterAKSUEvents();
	void RegisterAEKEvents();
	void RegisterAN94Events();
	void RegisterVIHREvents();
	void RegisterAKMEvents();
	void RegisterTKB0146Events();
	void RegisterFALEvents();
	void RegisterG3Events();
	void RegisterAUGEvents();
	void RegisterSVDEvents();
	void RegisterSVUEvents();
	void RegisterVSK94Events();
	void RegisterPSGEvents();
	void RegisterPKEvents();
	void RegisterRPOEvents();
	void RegisterRAILGUNEvents();
	void RegisterRGD5Events();
	void RegisterF1Events();
	void RegisterRGOEvents();
};
