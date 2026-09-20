/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#pragma once
#include "cdll_dll.h"

#define ITEM_FLAG_SELECTONEMPTY		1
#define ITEM_FLAG_NOAUTORELOAD		2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY	4
#define ITEM_FLAG_LIMITINWORLD		8
#define ITEM_FLAG_EXHAUSTIBLE		16 // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_NOCLIP			-1
#define WEAPON_ALLWEAPONS		(~(1<<WEAPON_SUIT))

// pre-defined vectors for weapon spread calculations
#define VECTOR_CONE_1DEGREES	Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES	Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES	Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES	Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES	Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES	Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES	Vector( 0.06105, 0.06105, 0.06105 )
#define VECTOR_CONE_8DEGREES	Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES	Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES	Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES	Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES	Vector( 0.17365, 0.17365, 0.17365 )

// weapon clip/carry ammo capacities
#define URANIUM_MAX_CARRY		100
#define	_9MM_MAX_CARRY			90
#define _357_MAX_CARRY			36
#define BUCKSHOT_MAX_CARRY		30
#define BOLT_MAX_CARRY			30
#define ROCKET_MAX_CARRY		5
#define HANDGRENADE_MAX_CARRY	5
#define SATCHEL_MAX_CARRY		5
#define TRIPMINE_MAX_CARRY		5
#define SNARK_MAX_CARRY			15
#define HORNET_MAX_CARRY		8
#define M203_GRENADE_MAX_CARRY	5
#define	_9X18_MAX_CARRY			90
#define	_9X21_MAX_CARRY			48
#define	_9X19_MAX_CARRY			90
#define	_545X18_MAX_CARRY		96
#define	FLARE_MAX_CARRY			10
#define	_12X70_MAX_CARRY		30
#define	_23X75_MAX_CARRY		16
#define	_762X39_MAX_CARRY		90
#define	_545X39_MAX_CARRY		90
#define	_556X45_MAX_CARRY		90
#define	_762X51_MAX_CARRY		60
#define	_9X39_MAX_CARRY			90
#define	_762X54_MAX_CARRY		30
#define RGD5_MAX_CARRY			2
#define F1_MAX_CARRY			2
#define RGO_MAX_CARRY			10
#define RAILGUN_MAX_CARRY		30
#define RPO_MAX_CARRY			1
// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, // glock
	BULLET_PLAYER_MP5, // mp5
	BULLET_PLAYER_357, // python
	BULLET_PLAYER_BUCKSHOT, // shotgun
	BULLET_PLAYER_CROWBAR, // crowbar swipe
	BULLET_PLAYER_PM,
	BULLET_PLAYER_APS,
	BULLET_PLAYER_GURZA,
	BULLET_PLAYER_GRACH,
	BULLET_PLAYER_OC33,
	BULLET_PLAYER_R92,
	BULLET_PLAYER_PB,
	BULLET_PLAYER_P96,
	BULLET_PLAYER_PSM,
	BULLET_PLAYER_OC23,
	BULLET_PLAYER_OC27,
	BULLET_PLAYER_22LR,
	BULLET_PLAYER_P7,
	BULLET_PLAYER_VP70,
	BULLET_PLAYER_KS23,
	BULLET_PLAYER_BEKAS,
	BULLET_PLAYER_VEPR,
	BULLET_PLAYER_SAIGA,
	BULLET_PLAYER_MC255,
	BULLET_PLAYER_BIZON,
	BULLET_PLAYER_KEDR,
	BULLET_PLAYER_VERESK,
	BULLET_PLAYER_KIPARIS,
	BULLET_PLAYER_GEPARD,
	BULLET_PLAYER_AKSU,
	BULLET_PLAYER_AEK,
	BULLET_PLAYER_AN94,
	BULLET_PLAYER_VIHR,
	BULLET_PLAYER_AKM,
	BULLET_PLAYER_TKB0146,
	BULLET_PLAYER_FAL,
	BULLET_PLAYER_G3,
	BULLET_PLAYER_AUG,
	BULLET_PLAYER_SVD,
	BULLET_PLAYER_SVU,
	BULLET_PLAYER_VSK94,
	BULLET_PLAYER_PSG,
	BULLET_PLAYER_PK,
	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,

} Bullet;

typedef struct
{
	int		iSlot;
	int		iPosition;
	const char	*pszAmmo1;	// ammo 1 type
	int		iMaxAmmo1;		// max ammo 1
	const char	*pszAmmo2;	// ammo 2 type
	int		iMaxAmmo2;		// max ammo 2
	const char	*pszName;
	int		iMaxClip;
	int		iId;
	int		iFlags;
	int		iWeight;// this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
	const char *pszName;
	int iId;
} AmmoInfo;
