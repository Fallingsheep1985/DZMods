    ref map<string, string>	m_AdminList    = new map<string, string>; //UID, name
	ref map<string, vector>	m_TPLocations  = new map<string, vector>; //name of town, pos
	ref map<string,int> 	m_ExtendedCommands = new map<string,int>;  //Command, length

	bool m_FreeCamera;
    bool m_IsDebugRunning = false;

    PlayerBase playerAdmin;
    PlayerIdentity identityT;
	string PlayerUID,GUID;

	void AdminTool()
	{
		Print("Class AdminTool:: Loading Scripted Mods...");

		//Add Towns to TP array
		m_TPLocations.Insert( "Severograd", "8428.0 0.0 12767.1" );
        m_TPLocations.Insert( "Krasnostav", "11172.0 0.0 12314.1" );
        m_TPLocations.Insert( "Mogilevka", "7537.8 0.0 5211.55" );
        m_TPLocations.Insert( "Stary", "6046.94 0.0 7733.97" );
        m_TPLocations.Insert( "Msta", "11322.55 0.0 5463.36" );
        m_TPLocations.Insert( "Vybor", "3784.16 0.0 8923.48" );
        m_TPLocations.Insert( "Gorka", "9514.27 0.0 8863.69" );
        m_TPLocations.Insert( "Solni", "13402.57 0.0 6303.35" );
        m_TPLocations.Insert( "NWAFS", "4540.52 0.0 9645.84" );
		m_TPLocations.Insert( "NWAFC", "4823.43 0.0 10457.16" );
		m_TPLocations.Insert( "NWAFN", "4214.84 0.0 10977.78" );
        m_TPLocations.Insert( "BAF", "4467.61 0.0 2496.14" );
        m_TPLocations.Insert( "NEAF", "11921.43 0.0 12525.55" );
        m_TPLocations.Insert( "ChernoC", "6649.22 0.0 2710.03" );
        m_TPLocations.Insert( "ChernoW", "6374.08 0.0 2361.01" );
        m_TPLocations.Insert( "ChernoE", "7331.70 0.0 2850.03" );
        m_TPLocations.Insert( "ElektroW", "10077.17 0.0 1988.65" );
        m_TPLocations.Insert( "ElektroE", "10553.55 0.0 2313.37" );
        m_TPLocations.Insert( "BerezC", "12319.54 0.0 9530.15" );
        m_TPLocations.Insert( "BerezS", "11991.42 0.0 9116.95" );
        m_TPLocations.Insert( "BerezN", "12823.14 0.0 10078.97" );
        m_TPLocations.Insert( "Svet", "13900.82 0.0 13258.12" );	
        m_TPLocations.Insert( "ZelenoS", "2572.80 0.0 5105.09" );
        m_TPLocations.Insert( "ZelenoN", "2741.48 0.0 5416.69" );
		m_TPLocations.Insert( "Lopatino", "2714.74 0.0 9996.33" );
		m_TPLocations.Insert( "Tisy", "1723.10 0.0 13983.88" );
		m_TPLocations.Insert( "Novaya", "3395.28 0.0 13013.61" );
		m_TPLocations.Insert( "Novy", "7085.73 0.0 7720.85" );
		m_TPLocations.Insert( "Grishino", "5952.15 0.0 10367.71" );
		m_TPLocations.Insert( "Kabanino", "5363.97 0.0 8594.39" );

		//Init Commands
		m_ExtendedCommands.Insert("/strip",6);
		m_ExtendedCommands.Insert("/tppm",5);
		m_ExtendedCommands.Insert("/tpp",4);
		m_ExtendedCommands.Insert("/tpto",5);
		m_ExtendedCommands.Insert("/spi",4);
		m_ExtendedCommands.Insert("/spg",4);
		m_ExtendedCommands.Insert("/tpc",4);
		//Sub commands
		m_ExtendedCommands.Insert("/export",1);
		m_ExtendedCommands.Insert("/ammo",1);
		m_ExtendedCommands.Insert("/stamina",1);
		m_ExtendedCommands.Insert("/LoadoutType",1);
		m_ExtendedCommands.Insert("/CustomLoadouts",1);
		m_ExtendedCommands.Insert("/SpawnArmed",1);
		m_ExtendedCommands.Insert("/updateLoadouts",1);
		m_ExtendedCommands.Insert("/freecam",1);
		m_ExtendedCommands.Insert("/debug",1);
		m_ExtendedCommands.Insert("/nighttime",1);
		m_ExtendedCommands.Insert("/daytime",1);
		m_ExtendedCommands.Insert("/godmode",1);
		m_ExtendedCommands.Insert("/heal",1);
		m_ExtendedCommands.Insert("/kill",1);
		m_ExtendedCommands.Insert("/tpalltome",1);
		m_ExtendedCommands.Insert("/killall",1);
		m_ExtendedCommands.Insert("/spawncar",1);
	}


	int TeleportAllPlayersTo(PlayerBase PlayerAdmin)
	{
		array<Man> players = new array<Man>;
	    GetGame().GetPlayers( players );

	    vector AdminPos;
	    AdminPos = PlayerAdmin.GetPosition();

	    for ( int i = 0; i < players.Count(); ++i )
		{
			PlayerBase Target = players.Get(i);
			Target.SetPosition( AdminPos );
		}
		return i;
	}

	void oSpawnItemFunc(bool ground, PlayerBase player, string ClassName)
	{
		EntityAI item;
		ItemBase itemBs

		vector NewPosition;
		vector OldPosition;

		if (ground)
		{
			OldPosition = player.GetPosition();

			NewPosition[0] = OldPosition[0] + 1.5;
			NewPosition[1] = OldPosition[1] + 0.1;
			NewPosition[2] = OldPosition[2] + 1.5;

			item = GetGame().CreateObject( ClassName, NewPosition, false, true );
		}else{

			item = player.GetInventory().CreateInInventory( ClassName );
			itemBs = ItemBase.Cast(item);	
			itemBs.SetQuantity(1);
		}
	}

	vector SnapToGround(vector pos)
    {
        float pos_x = pos[0];
        float pos_z = pos[2];
        float pos_y = GetGame().SurfaceY( pos_x, pos_z );
        vector tmp_pos = Vector( pos_x, pos_y, pos_z );
        tmp_pos[1] = tmp_pos[1] + pos[1];

        return tmp_pos;
    }

    void HideMessages()
	{
		if (playerAdmin != NULL)
		{
			array<Man> players = new array<Man>;
			GetGame().GetPlayers( players );
			
			for (int meme = 0; meme < players.Count(); ++meme)
			{
				PlayerBase memes = players.Get(meme);
				for (int nn = 0; nn < 12; ++nn)
				{
					if (memes.GetIdentity() != playerAdmin.GetIdentity())
					{
						Param1<string> Msgparam;
						Msgparam = new Param1<string>( "  " );
						GetGame().RPCSingleParam(memes, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, memes.GetIdentity());
					}
				}
			}
		}
	}

    ref array<string> CheckCommand(string CommandLine)
    {
    	ref array<string> ret = new array<string>;
    	string strRplce,mKey;
    	int cmdLength;

    	strRplce = CommandLine;

    	for (int i = 0; i < m_ExtendedCommands.Count(); ++i)
    	{
    		mKey 	  = m_ExtendedCommands.GetKey(i);
    		cmdLength = m_ExtendedCommands.Get(mKey);

    		if (CommandLine.Contains(mKey))
    		{
    			strRplce.Replace(mKey + " ","");
    			ret.Insert(mKey); //0 = Command 1 = Data
    			if (strRplce != "")
    			{
    				ret.Insert(strRplce);
    			}
    			return ret;
    		}
    	}
    	return NULL;
    }

	//---------------------------------------------------------------------------------
	override void OnEvent(EventType eventTypeId, Param params) 
	{
		super.OnEvent(eventTypeId,params);

		PlayerIdentity identity;
		PlayerBase player;

		switch(eventTypeId)
		{
			case ChatMessageEventTypeID:
	        ChatMessageEventParams chat_params = ChatMessageEventParams.Cast( params );

			array<Man> players = new array<Man>;
			GetGame().GetPlayers( players );

			string strMessage;
			PlayerBase selectedPlayer;
			PlayerIdentity selectedIdentity;

			Param1<string> Msgparam;
			if (chat_params.param1 == 0 && chat_params.param2 != "") //trigger only when channel is Global == 0 and Player Name does not equal to null
			{
			
			for ( int i = 0; i < players.Count(); ++i )
			{
				playerAdmin = players.Get(i);
				identityT = playerAdmin.GetIdentity();
				PlayerUID = identityT.GetPlainId();
				GUID = playerAdmin.GetIdentity().GetPlainId(); //Steam 64
				if (chat_params.param3.Contains("/"))
				{
					GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(this.HideMessages, 1, false);
				}
					if( m_AdminList.Contains(PlayerUID) && playerAdmin.GetIdentity().GetName() == chat_params.param2 && chat_params.param3.Contains("/"))
					{
						ref array<string> chatData = CheckCommand(chat_params.param3);
						string cCommand, cData;
						if (chatData != NULL)
						{
							cCommand = chatData.Get(0);
							cData    = chatData.Get(1);
						}else { cCommand = "UnknownCommand" }
				
						switch(cCommand)
						{
							case "/strip":
								for ( int a = 0; a < players.Count(); ++a )
								{
									selectedPlayer = players.Get(a);
									selectedIdentity = selectedPlayer.GetIdentity();
									if ( selectedIdentity.GetName() == cData )
									{
										selectedPlayer.RemoveAllItems();
										Msgparam = new Param1<string>( "Player "+cData +" Stripped!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
								}
							break;

							case "/tppm":
								for ( int zm = 0; zm < players.Count(); ++zm )
								{
									selectedPlayer = players.Get(zm);
									selectedIdentity = selectedPlayer.GetIdentity();
									if ( selectedIdentity.GetName() == cData )
									{
										playerAdmin.SetPosition(selectedPlayer.GetPosition());

										strMessage = "You were teleported to player " + cData;
										Msgparam = new Param1<string>( strMessage );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
								}
							break;

							case "/tpp":
								for ( int z = 0; z < players.Count(); ++z )
								{
									selectedPlayer = players.Get(z);
									selectedIdentity = selectedPlayer.GetIdentity();
									if ( selectedIdentity.GetName() == cData )
									{
										selectedPlayer.SetPosition(playerAdmin.GetPosition());

										Msgparam = new Param1<string>( "You were teleported by the admin!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, selectedIdentity);
											  
										strMessage = "Player " + cData + " was teleported to your location!";
										Msgparam = new Param1<string>( strMessage );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
								}
							break;

							case "/tpto":
								vector position = "0 0 0";
								if (m_TPLocations.Contains(cData))
								{
									m_TPLocations.Find( cData, position );

									vector ofixPlayerPos;
									ofixPlayerPos[0] = position[0];
									ofixPlayerPos[2] = position[2];

									ofixPlayerPos = SnapToGround( ofixPlayerPos );

									playerAdmin.SetPosition(ofixPlayerPos);

									strMessage = "Teleported To Location: " + cData;
									Msgparam = new Param1<string>( strMessage );
									GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								}
								else
								{
									strMessage = "Teleport Failed! Location: " + cData + " Is not on the list!";
									Msgparam = new Param1<string>( strMessage );
									GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								}
							break;

							case "/spi":
								oSpawnItemFunc(false,playerAdmin,cData);
								strMessage = "Admin Commands: Item " + cData + " Added in Inventory!";
								Msgparam = new Param1<string>( strMessage );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/spg":
								oSpawnItemFunc(true,playerAdmin,cData);
								strMessage = "Admin Commands: Item " + cData + " Spawned around you!";
								Msgparam = new Param1<string>( strMessage );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/tpc":
								vector tpPos = cData.ToVector();
								vector fixPlayerPos;
								fixPlayerPos[0] = tpPos[0];
								fixPlayerPos[2] = tpPos[2];

								fixPlayerPos = SnapToGround( fixPlayerPos );
								playerAdmin.SetPosition(fixPlayerPos);

								strMessage = "Teleported to:: " + tpPos;
								Msgparam = new Param1<string>( strMessage );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/export":
								ExportInventory(playerAdmin);
								Msgparam = new Param1<string>( "LOADOUT EXPORTED!" );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/ammo":
							EntityAI CurrentWeapon = playerAdmin.GetHumanInventory().GetEntityInHands();
							if( CurrentWeapon )
								{
									CurrentWeapon.SetHealth( CurrentWeapon.GetMaxHealth( "", "" ) );
									Magazine foundMag = ( Magazine ) CurrentWeapon.GetAttachmentByConfigTypeName( "DefaultMagazine" );
									if( foundMag && foundMag.IsMagazine())
									{
										foundMag.ServerSetAmmoMax();
									}
																		
									Object Suppressor = ( Object ) CurrentWeapon.GetAttachmentByConfigTypeName( "SuppressorBase" );
									if( Suppressor )
									{
									    Suppressor.SetHealth( Suppressor.GetMaxHealth( "", "" ) );
									}
									string displayName = CurrentWeapon.ConfigGetString("displayName");

									Msgparam = new Param1<string>( "Weapon " + displayName + "Reloaded and Repaired" );
									GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								}
							break;

							case "/stamina":
								if (m_StaminaStatus)
									{
										Msgparam = new Param1<string>( "Stamina ENABLED!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
										m_StaminaStatus = false;
									} else {
										Msgparam = new Param1<string>( "Stamina DISABLED!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
										m_StaminaStatus = true;
									}
							break;

							case "/LoadoutType":
								if (m_StaticLoadouts)
								{
									m_StaticLoadouts      = false;
									m_RandomizedLoadouts  = true;
									Msgparam = new Param1<string>( "Static Loadouts DISABLED! Randomized Loadouts ENABLED!" );
									GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								}
								else if (m_RandomizedLoadouts)
								{
									m_StaticLoadouts      = true;
									m_RandomizedLoadouts  = false;
									Msgparam = new Param1<string>( "Static Loadouts ENABLED Randomized Loadouts DISABLED!" );
									GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								}
							break;

							case "/CustomLoadouts":
								if (m_CustomLoadouts)
								    {
										m_CustomLoadouts = false;
										m_StaticLoadouts = false;
										m_RandomizedLoadouts  = false;
										Msgparam = new Param1<string>( "Custom Loadouts Disabled!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									} 
									else 
									{
										m_CustomLoadouts = true;
										m_RandomizedLoadouts  = true;
										Msgparam = new Param1<string>( "Custom Loadouts Enabled!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
							break;

							case "/SpawnArmed":
								 if (m_SpawnArmed)
								    {
										m_SpawnArmed = false;
										Msgparam = new Param1<string>( "New Spawn Players will NO LONGER Spawn with a GUN!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									} else {
										m_SpawnArmed = true;
										Msgparam = new Param1<string>( "New Spawn Players will SPAWN with a GUN!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
							break;

							case "/updateLoadouts":
								 ConstructLoadouts(true);
								 Msgparam = new Param1<string>( "Loadout Arrays UPDATED!" );
								 GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/freecam":
								 PlayerBase pBody = playerAdmin;

								 if (m_FreeCamera)
									{
										GetGame().SelectPlayer(playerAdmin.GetIdentity(), pBody);
										m_FreeCamera = false;
										Msgparam = new Param1<string>( "Exiting FreeCam!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}
									else
									{
										GetGame().SelectPlayer(playerAdmin.GetIdentity(), NULL);
										GetGame().SelectSpectator(playerAdmin.GetIdentity(), "freedebugcamera", playerAdmin.GetPosition());
										m_FreeCamera = true;
										Msgparam = new Param1<string>( "FreeCam Spawned!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
									}

							break;

							case "/debug":
								  if (m_IsDebugRunning)
									 {
										Msgparam = new Param1<string>( "DeBug Monitor (Status Monitor) Disabled!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
										GetGame().SetDebugMonitorEnabled(0);
										m_IsDebugRunning = false;
									 }
									 else
									 {
										Msgparam = new Param1<string>( "DeBug Monitor (Status Monitor) Enabled!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
										GetGame().SetDebugMonitorEnabled(1);
										m_IsDebugRunning = true;
									 }

							break;

							case "/nighttime":
								GetGame().GetWorld().SetDate( 1988, 9, 23, 22, 0 );
								Msgparam = new Param1<string>( "NIGHT TIME!!" );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/daytime":
								GetGame().GetWorld().SetDate( 1988, 5, 23, 12, 0 );
								Msgparam = new Param1<string>( "DAY TIME!!" );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;

							case "/godmode":
								string GmodeCheck;
								g_Game.GetProfileString("AdminGmode"+GUID,GmodeCheck);
								if (GmodeCheck == "true")
								   {
										g_Game.SetProfileString("AdminGmode"+GUID,"false");
										Msgparam = new Param1<string>( "God Mode DISABLED!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								   }
								   else
								   {
										g_Game.SetProfileString("AdminGmode"+GUID,"true");
										Msgparam = new Param1<string>( "God Mode ENABLED!" );
										GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								   }

							break;
																
							case "/heal":
								 Msgparam = new Param1<string>( "Player Healed!" );
								 GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
								 playerAdmin.SetHealth( playerAdmin.GetMaxHealth( "", "" ) );
								 playerAdmin.SetHealth( "","Blood", playerAdmin.GetMaxHealth( "", "Blood" ) );
							break;

							case "/kill":
								  playerAdmin.SetHealth(0);
							break;
																
							case "/tpalltome":
								 int tpCount = TeleportAllPlayersTo(playerAdmin);
								 string msgc = "All " + tpCount.ToString() + " Players Teleported to my POS!";
								 Msgparam = new Param1<string>( msgc );
								 GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;
																
							case "/killall":
								for ( int ig = 0; ig < players.Count(); ++ig )
								{
									PlayerBase Target = players.Get(ig);
									if ( Target.GetIdentity() != playerAdmin.GetIdentity() )
									{
										Target.SetHealth(0);						
									}
								}
							break;
							
							case "/spawncar":
							EntityAI MyV3S;
							vector NewPosition;
							vector OldPosition;
							OldPosition = playerAdmin.GetPosition();
							NewPosition[0] = OldPosition[0] + 1.5;
							NewPosition[1] = OldPosition[1] + 0.2;
							NewPosition[2] = OldPosition[2] + 1.5;
							MyV3S = GetGame().CreateObject( "OffroadHatchback", NewPosition, false, true, true );		            
							MyV3S.GetInventory().CreateAttachment("HatchbackHood");
							MyV3S.GetInventory().CreateAttachment("HatchbackTrunk");
							MyV3S.GetInventory().CreateAttachment("HatchbackDoors_CoDriver");
							MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
							MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
							MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
							MyV3S.GetInventory().CreateAttachment("HatchbackWheel");
							MyV3S.GetInventory().CreateAttachment("SparkPlug");
							MyV3S.GetInventory().CreateAttachment("EngineBelt");
							MyV3S.GetInventory().CreateAttachment("CarBattery");
							break;

							default:
								Msgparam = new Param1<string>( "Error: The following command is Unknown." );
								GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
							break;
					}
				}
			}
		}
	  break;
   }
}

void loadDebug()
{
	Param1<string> Msgparam;
	Msgparam = new Param1<string>( "DeBug Monitor (Status Monitor) Enabled!" );
	GetGame().RPCSingleParam(playerAdmin, ERPCs.RPC_USER_ACTION_MESSAGE, Msgparam, true, playerAdmin.GetIdentity());
	GetGame().SetDebugMonitorEnabled(1);
	m_IsDebugRunning = true;
}