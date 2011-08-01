#include <sourcemod>
#include <sdktools>
#include <cstrike>
#include <sdkhooks>

#pragma semicolon 1

#define PLUGIN_VERSION "1.0.0"

new Handle:cvar_zombie_team;
new String:zombie_team[8];

public Plugin:myinfo = 
{
	name = "Zombie Riot Tools",
	author = "Dan \"Chimera\" Cassey",
	description = "Extention plugin for Zombie Riot",
	version = PLUGIN_VERSION,
	url = "http://forum.i3d.net/"
};

public OnPluginStart()
{
	HookEvent("player_spawn", Event_PlayerSpawn);
	cvar_zombie_team = FindConVar("zriot_zombieteam");
}

public OnPlayerPutInServer(client)
{
	SDKHook(client, SDKHook_WeaponEquip, OnWeaponEquip);
}

/**
 * On player spawn, if the player is a zombie, strip it of weapons and give it a 
 * knife. (prevents a buildup of glocks)
 */
public Event_PlayerSpawn(Handle:event, const String:name[], bool:dontBroadcast)
{
	new userid = GetEventInt(event, "userid");
	new client = GetClientOfUserId(userid);
	
	if ( (IsClientInGame(client)) && (IsPlayerAlive(client)) && (cvar_zombie_team != INVALID_HANDLE) )
	{
		GetConVarString(cvar_zombie_team, zombie_team, sizeof(zombie_team));
		
		// If this client is on the zombie team
		if ((StrEqual(zombie_team, "t") && (GetClientTeam(client) == CS_TEAM_T))
		|| (StrEqual(zombie_team, "ct") && (GetClientTeam(client) == CS_TEAM_CT)) )
		{
			
			new wepIdx;
	
			// strip all weapons
			for (new s = 0; s < 4; s++)
			{
				if ((wepIdx = GetPlayerWeaponSlot(client, s)) != -1)
				{
					RemovePlayerItem(client, wepIdx);
					RemoveEdict(wepIdx);
				}
			}
			
			// give the player a knife
			GivePlayerItem(client, "weapon_knife");
		}
	}
}

public Action:OnWeaponEquip(client, weapon)
{
	new String:weapon_name[32];
	GetEdictClassname(weapon, weapon_name, sizeof(weapon_name));
	if (!StrEqual(weapon_name, "weapon_knife"))
	{
		if ( (IsClientInGame(client)) && (IsPlayerAlive(client)) && (cvar_zombie_team != INVALID_HANDLE) )
		{
			GetConVarString(cvar_zombie_team, zombie_team, sizeof(zombie_team));
			
			// If this client is on the zombie team
			if ((StrEqual(zombie_team, "t") && (GetClientTeam(client) == CS_TEAM_T))
			|| (StrEqual(zombie_team, "ct") && (GetClientTeam(client) == CS_TEAM_CT)) )
			{
				
				new wepIdx;
		
				// strip all weapons
				for (new s = 0; s < 4; s++)
				{
					if ((wepIdx = GetPlayerWeaponSlot(client, s)) != -1)
					{
						RemovePlayerItem(client, wepIdx);
						RemoveEdict(wepIdx);
					}
				}
				
				// give the player a knife
				GivePlayerItem(client, "weapon_knife");
			}
		}
	}
}