#include "gsc_player.hpp"
#include "include.hpp"

int playerStates = 0x084BD120;
int sizeOfPlayer = 0x3184;
int gentities = 0x0841FFE0;
int gentities_size = 628;

int playerinfo_base = 0x090B4F8C;
int playerinfo_size = 0x2958F;

#define PLAYERSTATE(playerid) (playerStates + playerid * sizeOfPlayer)
#define PLAYERBASE(playerid) (playerinfo_base + playerid * playerinfo_size)
#define MAX_SAVES 512
#define MAX_TRIGGER_COUNT 512
#define MAX_CP_COUNT 512
#define MAX_PLAYERS 64
#define CHAR_BIT 8
#define MAX_ENDER_COUNT 16

void gsc_client_download(int id)
{
	client_t* cl = Plugin_GetClientForClientNum(id);
	char* dlname = Plugin_Scr_GetString(0);
	strncpy(cl->downloadName, dlname, 64);
	cl->downloadName[64] = '\0';
}

void gsc_setdemorecording(int id)
{
	client_t* c = Plugin_GetClientForClientNum(id);
	c->demorecording = (qboolean)Plugin_Scr_GetInt(0);
	if(c->demorecording)
		c->demoDeltaFrameCount = 0;
}

void gsc_getQueuedReliableMessages(int id)
{
	client_t* c = Plugin_GetClientForClientNum(id);
	int count = c->reliableSequence - c->reliableAcknowledge;
	Plugin_Scr_AddInt(count);
}

unsigned int jh_fps[64];
void gsc_resetfps(int id)
{
	client_t* c = Plugin_GetClientForClientNum(id);
	jh_fps[id] = c->clFrames;
}

void gsc_getfps(int id)
{
	client_t* c = Plugin_GetClientForClientNum(id);
	int fps = 20 * (c->clFrames - jh_fps[id]);
	Plugin_Scr_AddInt(fps);
}

void gsc_printcommandtime(int id)
{
	//playerState_t* ps = Plugin_SV_GameClientNum(id);
	//Plugin_Printf("command time: %d\n", ps->commandTime);
	client_t* c = Plugin_GetClientForClientNum(id);
	if(c->clFPS == 0)
		c->clFPS = Plugin_Scr_GetInt(0);
	else
	{
		c->clFrameCalcTime += Plugin_Scr_GetInt(0) - c->clFPS;
		c->clFPS = 0;
	}
}

void Gsc_Player_Velocity_Add(int id) {
    vec3_t velocity;
    playerState_t* ps;

    Plugin_Scr_GetVector(0, &velocity);

    ps = Plugin_SV_GameClientNum(id);

    ps->velocity[0] += velocity[0];
    ps->velocity[1] += velocity[1];
    ps->velocity[2] += velocity[2];

    Plugin_Scr_AddInt(1);
}

void Gsc_Player_GetIP(int id){
    client_t* cl;
	char address[128] = {0};

    cl = Plugin_GetClientForClientNum(id);

    Plugin_NET_AdrToStringMT(&cl->netchan.remoteAddress, address, sizeof(address));

    Plugin_Scr_AddString((char *)address);
}

void gsc_player_issprinting(int id)
{
	playerState_t* ps = Plugin_SV_GameClientNum(id);
	if(ps->sprintState.lastSprintStart <= ps->sprintState.lastSprintEnd)
		Plugin_Scr_AddInt(0);
	else
		Plugin_Scr_AddInt(1);
}
