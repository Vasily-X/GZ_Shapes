#if defined _gz_circles_included
	#endinput
#endif
#define _gz_circles_included


// ================================== [DEFINITIONS] ================================== //
#if !defined MAX_GZ_CIRCLES
	#define MAX_GZ_CIRCLES			10
#endif

#if !defined INVALID_GZ_CIRCLE_ID
	#define INVALID_GZ_CIRCLE_ID	-1
#endif


// ================================== [VARIABLES] ================================== //
enum gzc_data
{
	Float:gzc_x,
	Float:gzc_y,
	Float:gzc_radius,
	gzc_Elements[MAX_GANG_ZONES],
	bool:gzc_Exists
}
static 
	gzc_gInfo[MAX_GZ_CIRCLES][gzc_data],
	bool:gzc_gInit,
	bool:gzc_gExit,
	gzc_gGangZonesUsed;


// ================================== [HOOKED CALLBACKS] ================================== //
public OnFilterScriptInit()
{
    if(!gzc_gInit)
    {
    	gzc_gInit = true;
    	gzc_gGangZonesUsed = 0;

      	for(new x = 0; x < MAX_GZ_CIRCLES; x++)
      	{
      		for(new y = 0; y < MAX_GANG_ZONES; y++)
      		{
      			gzc_gInfo[x][gzc_Elements][y] = INVALID_GANG_ZONE;
      		}
      	}
    }

    #if defined gzc_OnFilterScriptInit
         return gzc_OnFilterScriptInit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit gzc_OnFilterScriptInit
#if defined gzc_OnFilterScriptInit
	forward gzc_OnFilterScriptInit();
#endif

public OnFilterScriptExit()
{
    if(!gzc_gExit)
    {
    	gzc_gExit = true;
    	GZ_CircleDestroyAll();
    }

    #if defined gzc_OnFilterScriptExit
         return gzc_OnFilterScriptExit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit gzc_OnFilterScriptExit
#if defined gzc_OnFilterScriptExit
	forward gzc_OnFilterScriptExit();
#endif

public OnGameModeInit()
{
 	if(!gzc_gInit)
    {
    	gzc_gInit = true;
    	gzc_gGangZonesUsed = 0;

      	for(new x = 0; x < MAX_GZ_CIRCLES; x++)
      	{
      		for(new y = 0; y < MAX_GANG_ZONES; y++)
      		{
      			gzc_gInfo[x][gzc_Elements][y] = INVALID_GANG_ZONE;
      		}
      	}
    }
    #if defined gzc_OnGameModeInit
         return gzc_OnGameModeInit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit gzc_OnGameModeInit
#if defined gzc_OnGameModeInit
	forward gzc_OnGameModeInit();
#endif

public OnGameModeExit()
{
 	if(!gzc_gExit)
    {
    	gzc_gExit = true;
    	GZ_CircleDestroyAll();
    }

    #if defined gzc_OnGameModeExit
         return gzc_OnGameModeExit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit gzc_OnGameModeExit
#if defined gzc_OnGameModeExit
	forward gzc_OnGameModeExit();
#endif


// ================================== [NEW FUNCTIONS] ================================== //
stock GZ_CircumferenceCreate(Float:x, Float:y, Float:radius, squares = -1, Float:square_size = 5.0, Float:min_ang = 0.0, Float:max_ang = 360.0)
{
	for(new i = 0; i < MAX_GZ_CIRCLES; i++)
	{
		if(gzc_gInfo[i][gzc_Exists])
		{
			continue;
		}

		new auto_squares = floatround((radius * 512.0)/750.0);
		new free_squares = MAX_GANG_ZONES - gzc_gGangZonesUsed;

		squares = (squares == -1) ? ( (auto_squares <= free_squares) ? (auto_squares) : (free_squares) ) : ( (squares <= free_squares) ? (squares) : (free_squares) );

		if(squares == 0)
		{
			printf("** [GangZone Circles] The circumference couldn't be created, gang zones limit reached.");
			break;
		}

		new Float:temp_x,
			Float:temp_y,
			Float:temp_diff,
			Float:diff = (max_ang - min_ang) / float(squares);

		for(new j = 0; j < squares; j++)
		{
			temp_diff = min_ang + (diff * float(j));

			temp_x = radius * floatcos(temp_diff, degrees);
			temp_y = radius * floatsin(temp_diff, degrees);

			gzc_gInfo[i][gzc_Elements][j] = GangZoneCreate((x + temp_x) - square_size, (y + temp_y) - square_size, (x + temp_x) + square_size, (y + temp_y) + square_size);
		}

		gzc_gGangZonesUsed += squares;
		gzc_gInfo[i][gzc_Exists] = true;

		return i;
	}
	return INVALID_GZ_CIRCLE_ID;
}

stock GZ_CircleCreate(Float:x, Float:y, Float:radius, squares = -1)
{
	for(new i = 0; i < MAX_GZ_CIRCLES; i++)
	{
		if(gzc_gInfo[i][gzc_Exists])
		{
			continue;
		}

		new 	
			auto_squares = floatround((radius * 512.0)/750.0),
			free_squares = MAX_GANG_ZONES - gzc_gGangZonesUsed;

		squares = (squares == -1) ? ( (auto_squares <= free_squares) ? (auto_squares) : (free_squares) ) : ( (squares <= free_squares) ? (squares) : (free_squares) );

		if(squares == 0)
		{
			printf("** [GangZone Circles] The circle couldn't be created, gang zones limit reached.");
			break;
		}

		new 
			Float:temp_x,
			Float:temp_y,
			Float:next_x,
			Float:temp_diff,
			Float:diff = 360.0 / float(squares);

		for(new j = 0; j < squares; j++)
		{
			temp_diff = (diff * float(j));

			temp_x = radius * floatcos(temp_diff, degrees);
			temp_y = radius * floatsin(temp_diff, degrees);
			next_x = radius * floatcos((diff * float(j + 1)), degrees);

			if(0.0 <= temp_diff <= 180.0)
			{
				gzc_gInfo[i][gzc_Elements][j] =  GangZoneCreate((x + next_x), y, (x + temp_x), (y + temp_y));
			}
			else
			{
				gzc_gInfo[i][gzc_Elements][j] =  GangZoneCreate((x + temp_x), (y + temp_y), (x + next_x), y);
			}
		}

		gzc_gGangZonesUsed += auto_squares;
		gzc_gInfo[i][gzc_Exists] = true;

		return i;
	}
	return INVALID_GZ_CIRCLE_ID;
}

stock GZ_CircleDestroy(circleid)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				gzc_gGangZonesUsed--;

				GangZoneDestroy(gzc_gInfo[circleid][gzc_Elements][y]);
				gzc_gInfo[circleid][gzc_Elements][y] = INVALID_GANG_ZONE;
			}

			gzc_gInfo[circleid][gzc_Exists] = false;

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleShowForPlayer(playerid, circleid, color)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneShowForPlayer(playerid, gzc_gInfo[circleid][gzc_Elements][y], color);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleShowForAll(circleid, color)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneShowForAll(gzc_gInfo[circleid][gzc_Elements][y], color);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleHideForPlayer(playerid, circleid)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneHideForPlayer(playerid, gzc_gInfo[circleid][gzc_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleHideForAll(circleid)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneHideForAll(playerid, gzc_gInfo[circleid][gzc_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleFlashForPlayer(playerid, circleid, flashcolor)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneFlashForPlayer(playerid, gzc_gInfo[circleid][gzc_Elements][y], flashcolor);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleFlashForAll(circleid, flashcolor)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneFlashForAll(gzc_gInfo[circleid][gzc_Elements][y], flashcolor);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleStopFlashForPlayer(playerid, circleid)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneStopFlashForPlayer(playerid, gzc_gInfo[circleid][gzc_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleStopFlashForAll(circleid)
{
	if(0 <= circleid < MAX_GZ_CIRCLES)
	{
		if(gzc_gInfo[circleid][gzc_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzc_gInfo[circleid][gzc_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneStopFlashForAll(gzc_gInfo[circleid][gzc_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleDestroyAll()
{
	for(new x = 0; x < MAX_GZ_CIRCLES; x++)
	{
		if(!gzc_gInfo[x][gzc_Exists])
		{
			continue;
		}

		for(new y = 0; y < MAX_GANG_ZONES; y++)
  		{
  			if(gzc_gInfo[x][gzc_Elements][y] == INVALID_GANG_ZONE)
  			{
  				continue;
  			}

  			GangZoneDestroy(gzc_gInfo[x][gzc_Elements][y]);
  		}

  		gzc_gInfo[x][gzc_Exists] = false;
	}
	return 1;
}
