#if defined _gz_shapes_included
	#endinput
#endif
#define _gz_shapes_included


// ================================== [DEFINITIONS] ================================== //
#if !defined MAX_GZ_SHAPES
	#define MAX_GZ_SHAPES				10
#endif

#if !defined INVALID_GZ_SHAPE_ID
	#define INVALID_GZ_SHAPE_ID			-1
#endif


// ================================== [VARIABLES] ================================== //
static bool:gzs_gInit;
static bool:gzs_gExit;

enum GZ_SHAPE_TYPES
{
	CIRCLE,
	EMPTY_CIRCLE,
	CIRCUMFERENCE
}

enum gzs_data
{
	Float:gzs_x,
	Float:gzs_y,
	Float:gzs_radius,
	gzs_Elements[MAX_GANG_ZONES],
	bool:gzs_Exists
}
static gzs_gInfo[MAX_GZ_SHAPES][gzs_data];

static gzs_gGangZonesUsed;


// ================================== [HOOKED CALLBACKS] ================================== //
public OnFilterScriptInit()
{
    if(!gzs_gInit)
    {
    	gzs_gInit = true;
    	gzs_gGangZonesUsed = 0;

      	for(new x = 0; x < MAX_GZ_SHAPES; x++)
      	{
      		for(new y = 0; y < MAX_GANG_ZONES; y++)
      		{
      			gzs_gInfo[x][gzs_Elements][y] = INVALID_GANG_ZONE;
      		}
      	}
    }

    #if defined gzs_OnFilterScriptInit
         return gzs_OnFilterScriptInit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnFilterScriptInit
	#undef OnFilterScriptInit
#else
	#define _ALS_OnFilterScriptInit
#endif
#define OnFilterScriptInit gzs_OnFilterScriptInit
#if defined gzs_OnFilterScriptInit
	forward gzs_OnFilterScriptInit();
#endif

public OnFilterScriptExit()
{
    if(!gzs_gExit)
    {
    	gzs_gExit = true;
    	GZ_CircleDestroyAll();
    }

    #if defined gzs_OnFilterScriptExit
         return gzs_OnFilterScriptExit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnFilterScriptExit
	#undef OnFilterScriptExit
#else
	#define _ALS_OnFilterScriptExit
#endif
#define OnFilterScriptExit gzs_OnFilterScriptExit
#if defined gzs_OnFilterScriptExit
	forward gzs_OnFilterScriptExit();
#endif

public OnGameModeInit()
{
 	if(!gzs_gInit)
    {
    	gzs_gInit = true;
    	gzs_gGangZonesUsed = 0;

      	for(new x = 0; x < MAX_GZ_SHAPES; x++)
      	{
      		for(new y = 0; y < MAX_GANG_ZONES; y++)
      		{
      			gzs_gInfo[x][gzs_Elements][y] = INVALID_GANG_ZONE;
      		}
      	}
    }
    #if defined gzs_OnGameModeInit
         return gzs_OnGameModeInit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnGameModeInit
	#undef OnGameModeInit
#else
	#define _ALS_OnGameModeInit
#endif
#define OnGameModeInit gzs_OnGameModeInit
#if defined gzs_OnGameModeInit
	forward gzs_OnGameModeInit();
#endif

public OnGameModeExit()
{
 	if(!gzs_gExit)
    {
    	gzs_gExit = true;
    	GZ_CircleDestroyAll();
    }

    #if defined gzs_OnGameModeExit
         return gzs_OnGameModeExit();
    #else
   	 	return 1;
	#endif
}
#if defined _ALS_OnGameModeExit
	#undef OnGameModeExit
#else
	#define _ALS_OnGameModeExit
#endif
#define OnGameModeExit gzs_OnGameModeExit
#if defined gzs_OnGameModeExit
	forward gzs_OnGameModeExit();
#endif


// ================================== [NEW FUNCTIONS] ================================== //
stock GZ_ShapeCreate(GZ_SHAPE_TYPES:type, Float:x, Float:y, Float:radius, squares = -1, Float:square_size = 5.0, Float:min_ang = 0.0, Float:max_ang = 360.0)
{
	for(new i = 0; i < MAX_GZ_SHAPES; i++)
	{
		if(gzs_gInfo[i][gzs_Exists])
		{
			continue;
		}

		new auto_squares = floatround((radius * 512.0)/750.0);
		new free_squares = MAX_GANG_ZONES - gzs_gGangZonesUsed;

		squares = (squares == -1) ? ( (auto_squares <= free_squares) ? (auto_squares) : (free_squares) ) : ( (squares <= free_squares) ? (squares) : (free_squares) );

		if(squares == 0)
		{
			printf("** [GZ Shapes] The shape couldn't be created, gang zones limit reached.");
			break;
		}

		new Float:temp_x,
		new Float:temp_y,
		new Float:temp_diff,
		new Float:diff = (max_ang - min_ang) / float(squares);

		switch(type)
		{
			case CIRCLE:
			{
				for(new j = 0; j < squares; j++)
				{
					temp_diff = (diff * float(j));

					temp_x = radius * floatcos(temp_diff, degrees);
					temp_y = radius * floatsin(temp_diff, degrees);
					next_x = radius * floatcos((diff * float(j + 1)), degrees);

					if(0.0 <= temp_diff <= 180.0)
					{
						gzs_gInfo[i][gzs_Elements][j] =  GangZoneCreate((x + next_x), y, (x + temp_x), (y + temp_y));
					}
					else
					{
						gzs_gInfo[i][gzs_Elements][j] =  GangZoneCreate((x + temp_x), (y + temp_y), (x + next_x), y);
					}
				}
			}
			case EMPTY_CIRCLE:
			{
			}
			case CIRCUMFERENCE:
			{
				for(new j = 0; j < squares; j++)
				{
					temp_diff = min_ang + (diff * float(j));

					temp_x = radius * floatcos(temp_diff, degrees);
					temp_y = radius * floatsin(temp_diff, degrees);

					gzs_gInfo[i][gzs_Elements][j] = GangZoneCreate((x + temp_x) - square_size, (y + temp_y) - square_size, (x + temp_x) + square_size, (y + temp_y) + square_size);
				}
			}
			case default:
			{
				printf("** [GZ Shapes] The shape couldn't be created, the shape type isn't correct.");
				break;
			}
		}

		gzs_gGangZonesUsed += squares;
		gzs_gInfo[i][gzs_Exists] = true;

		return i;
	}
	return INVALID_GZ_SHAPE_ID;
}

stock GZ_CircleDestroy(circleid)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				gzs_gGangZonesUsed--;

				GangZoneDestroy(gzs_gInfo[circleid][gzs_Elements][y]);
				gzs_gInfo[circleid][gzs_Elements][y] = INVALID_GANG_ZONE;
			}

			gzs_gInfo[circleid][gzs_Exists] = false;

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleShowForPlayer(playerid, circleid, color)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneShowForPlayer(playerid, gzs_gInfo[circleid][gzs_Elements][y], color);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleShowForAll(circleid, color)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneShowForAll(gzs_gInfo[circleid][gzs_Elements][y], color);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleHideForPlayer(playerid, circleid)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneHideForPlayer(playerid, gzs_gInfo[circleid][gzs_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleHideForAll(circleid)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneHideForAll(playerid, gzs_gInfo[circleid][gzs_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleFlashForPlayer(playerid, circleid, flashcolor)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneFlashForPlayer(playerid, gzs_gInfo[circleid][gzs_Elements][y], flashcolor);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleFlashForAll(circleid, flashcolor)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneFlashForAll(gzs_gInfo[circleid][gzs_Elements][y], flashcolor);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleStopFlashForPlayer(playerid, circleid)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneStopFlashForPlayer(playerid, gzs_gInfo[circleid][gzs_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleStopFlashForAll(circleid)
{
	if(0 <= circleid < MAX_GZ_SHAPES)
	{
		if(gzs_gInfo[circleid][gzs_Exists])
		{
			for(new y = 0; y < MAX_GANG_ZONES; y++)
			{
				if(gzs_gInfo[circleid][gzs_Elements][y] == INVALID_GANG_ZONE)
				{
					continue;
				}

				GangZoneStopFlashForAll(gzs_gInfo[circleid][gzs_Elements][y]);
			}

			return 1;
		}
	}
	return 0;
}

stock GZ_CircleDestroyAll()
{
	for(new x = 0; x < MAX_GZ_SHAPES; x++)
	{
		if(!gzs_gInfo[x][gzs_Exists])
		{
			continue;
		}

		for(new y = 0; y < MAX_GANG_ZONES; y++)
  		{
  			if(gzs_gInfo[x][gzs_Elements][y] == INVALID_GANG_ZONE)
  			{
  				continue;
  			}

  			GangZoneDestroy(gzs_gInfo[x][gzs_Elements][y]);
  		}

  		gzs_gInfo[x][gzs_Exists] = false;
	}
	return 1;
}
