/*
 * 	Copyright (c) 2016 Arkanite
 *
 *	 	-A Native HomeBrew Browser for the Playstation Vita-
 *
 *	This program is free software:
 *		you can redistribute it and/or modify it under the terms of the
 *		GNU General Public License as published by the Free Software Foundation,
 *		either version 3 of the License, or (at your option) any later version.
 *		
 *		This program comes WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		You should have received a copy of the GNU General Public License
 *		along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */
 

// LIBRARIES
#include "vhbb.h"
// DEFINE MACROS
#include "macros.h"
// INCLUDE NETWORK
#include "network.h"
// INCLUDE TOOLS
#include "tools.h"
// INITIALIZE VARIABLES
#include "init_var.h"


// MAIN -------------------------------------------------
//-------------------------------------------------------

int main()
	{
	// CREATE DIRECTORIES
	dir_create( "ux0:/data/VitaHbBrowser/sys/" 		 );
	dir_create( "ux0:/data/VitaHbBrowser/sys/vers/"  );
	dir_create( "ux0:/data/VitaHbBrowser/dta/icons/" );
	dir_create( "ux0:/data/VitaHbBrowser/dta/prevs/" );
	dir_create( "ux0:/data/VitaHbBrowser/dta/files/" );
	
	// INITIALIZE VITA2D
	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x40, 0x40, 0x40, 0xFF ) );
	
	// LOAD DEFAULT FONT
	vita2d_font *font_default 						= vita2d_load_font_mem( fontDefault, fontDefault_size );
	
	// SET TOUCH SAMPLING
	sceTouchSetSamplingState( SCE_TOUCH_PORT_FRONT, 1 );
	sceTouchSetSamplingState( SCE_TOUCH_PORT_BACK,  1 );
	
	// INITIALIZE SPRITE POINTERS
	#include "init_sprites.h"
	
	memset( &pad, 0, sizeof( pad ) );
	
	// SHOW SPLASH BACKGROUND
	vita2d_start_drawing();
	vita2d_clear_screen();
	vita2d_draw_texture( img_splash, 0, 0 );
	vita2d_end_drawing();
	vita2d_swap_buffers();
	
	// BOOT -------------------------------------------------
	logcat_add( "booting...", "", "\n" );
	// CHECK FOR FIRST LOAD
	if ( access( VHBB_APP_FILE_SYSTEMFILE, F_OK ) == -1 )
		{
		// FIRST BOOT
		logcat_add( "-FIRST LOAD-\n", "creating system config file", "\n" );
		FILE *fp 	= fopen( VHBB_APP_FILE_SYSTEMFILE, "ab+" );
		fprintf( fp, "***********************************\n" );
		fprintf( fp, "-Vita HomeBrew Browser-" );
		fprintf( fp, " ver. " );
		fprintf( fp, VHBB_VERSION );
		fprintf( fp, "\n Created By\n" );
		fprintf( fp, "      Arkanite\n" );
		fprintf( fp, "***********************************\n\n" );
		fprintf( fp, VHBB_VERSION );
		fclose( fp );
		logcat_add( "config file created", "\n", "" );
		}
	else
		{
		// LOAD USER SETTINGS
		
		}
	#include "database_load.h"
	
////MAIN LOOP
	int i;
	int dPadTimer	= 0;
	#include "vpkDownload.h"
	while ( 1 )
		{
		sceKernelPowerTick(0);
		////////////////////////////////// controls
		#include "ctrls.h"
		
		// SETTINGS
		if ( settingsOpen == 1 )
			{
			settingsDraw		= 1;
			if ( originX < 192 ) 	{ originX += 15; }
			else 				  	{ originX = 207; }
			}
		else
			{
			if ( originX > 15 ) 	{ originX -= 15; }
			else 				 	{ originX	= 0; settingsDraw = 0; }
			}
		
		/// SCROLLING -------------------------------------------------
		#include "scroll.h"
		///------------------------------------------------------------
		
		
		/// TOUCH CHECK PRESSED
		if ( touch_check_pressed == 1 )
			{
			if ( dialogOpen == 0 )
				{
				if ( settingsOpen == 0 )
					{
					if ( touch_y < 90 )
						{
						// TOPBAR BUTTONS
						if ( point_in_rectangle( touch_x, touch_y,   1, 1,  98, 90 ) == 1 && screen != 0 ) { screen = 0; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y,  99, 1, 204, 90 ) == 1 && screen != 1 ) { screen = 1; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 205, 1, 326, 90 ) == 1 && screen != 2 ) { screen = 2; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 327, 1, 427, 90 ) == 1 && screen != 3 ) { screen = 3; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 428, 1, 552, 90 ) == 1 && screen != 4 ) { screen = 4; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 553, 1, 674, 90 ) == 1 && screen != 5 ) { screen = 5; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 675, 1, 799, 90 ) == 1 && screen != 6 ) { screen = 6; originY = 0; previewActive = 0; }
						if ( point_in_rectangle( touch_x, touch_y, 800, 1, 960, 90 ) == 1 ) { btnState_search = 1; }
						}
					else
						{
						if ( previewActive == 1 )
							{
							// DOWNLOAD/INSTALL/UPDATE PRESSED
							if ( point_in_rectangle( touch_x, touch_y, 197, 213, 357, 266 ) == 1 )
								{
								btnState_previewAction		= 1;
								}
							}
						}
					}
				else
					{
					if ( settingsPosY >= 207 )
						{
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +110), 60, (settingsPosY +222) ) == 1 ) { btnState_settingsSound   = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +245), 60, (settingsPosY +357) ) == 1 ) { btnState_settingsCtrls  = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +380), 60, (settingsPosY +492) ) == 1 ) { btnState_settingsContact  = 1; }
						if ( point_in_rectangle( touch_x, touch_y, 40, (settingsPosY +515), 60, (settingsPosY +627) ) == 1 ) { btnState_settingsAbout = 1; }
						}
					
					
					}
				}
			else
				{
				if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) == 1 )
					{
					btnState_dialogBtn2		= 1;
					}
				}
			}
		
		/// BUTTON CHECK
		if ( touch_check == 1 && previewActive == 0 && settingsDraw == 0 )
			{
			if ( touch_y > 90  && clickable == 1 ) 	{ itemPressed = floor((touch_y +abs(originY)) /itemPanelHeight) -1; }
			else									{ itemPressed = -1; }
			}
		else { /*itemPressed = -1; */ }
			
		/// BUTTON CHECK RELEASE
		if ( touch_check_released == 1 )
			{
			if ( dialogOpen == 0 )
				{
				if ( settingsOpen == 0 )
					{
					if ( previewActive == 0 )
						{
						if ( itemPressed >= 0 && clickable == 1 )
							{
							int previewReady	= 1;
							switch ( screen )
								{
								case 0:			// NEW
												if ( itemPressed < 0 || itemPressed > itemCount_new ) { previewReady = 0; break; }
												previewCategory		= catListNew[itemPressed].cat;
												strcpy( previewName, 		catListNew[itemPressed].name 		);
												strcpy( previewAuthor, 		catListNew[itemPressed].author 		);
												strcpy( previewVersion, 	catListNew[itemPressed].version 	);
												strcpy( previewDlSize, 		catListNew[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListNew[itemPressed].release 	);
												strcpy( previewDescription, catListNew[itemPressed].description );
												strcpy( previewLink, 		catListNew[itemPressed].link 		);
												strcpy( previewDir, 		catListNew[itemPressed].dir 		);
												previewEbootSize		= catListNew[itemPressed].ebootSize;
												break;
								case 1:			// APPS
												if ( itemPressed < 0 || itemPressed > itemCount_apps ) { previewReady = 0; break; }
												previewCategory		= catListApps[itemPressed].cat;
												strcpy( previewName, 		catListApps[itemPressed].name 			);
												strcpy( previewAuthor, 		catListApps[itemPressed].author 		);
												strcpy( previewVersion, 	catListApps[itemPressed].version 		);
												strcpy( previewDlSize, 		catListApps[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListApps[itemPressed].release 		);
												strcpy( previewDescription, catListApps[itemPressed].description 	);
												strcpy( previewLink, 		catListApps[itemPressed].link 			);
												strcpy( previewDir, 		catListApps[itemPressed].dir 			);
												previewEbootSize		= catListApps[itemPressed].ebootSize;
												break;
								case 2:			// GAMES
												if ( itemPressed < 0 || itemPressed > itemCount_games ) { previewReady = 0; break; }
												previewCategory		= catListGames[itemPressed].cat;
												strcpy( previewName, 		catListGames[itemPressed].name 			);
												strcpy( previewAuthor, 		catListGames[itemPressed].author 		);
												strcpy( previewVersion, 	catListGames[itemPressed].version 		);
												strcpy( previewDlSize, 		catListGames[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListGames[itemPressed].release 		);
												strcpy( previewDescription, catListGames[itemPressed].description 	);
												strcpy( previewLink, 		catListGames[itemPressed].link 			);
												strcpy( previewDir, 		catListGames[itemPressed].dir 			);
												previewEbootSize		= catListGames[itemPressed].ebootSize;
												break;
								case 3:			// EMULATORS
												if ( itemPressed < 0 || itemPressed > itemCount_emu ) { previewReady = 0; break; }
												previewCategory		= catListEmulators[itemPressed].cat;
												strcpy( previewName, 		catListEmulators[itemPressed].name 			);
												strcpy( previewAuthor, 		catListEmulators[itemPressed].author 		);
												strcpy( previewVersion, 	catListEmulators[itemPressed].version 		);
												strcpy( previewDlSize, 		catListEmulators[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListEmulators[itemPressed].release 		);
												strcpy( previewDescription, catListEmulators[itemPressed].description 	);
												strcpy( previewLink, 		catListEmulators[itemPressed].link 			);
												strcpy( previewDir, 		catListEmulators[itemPressed].dir 			);
												previewEbootSize		= catListEmulators[itemPressed].ebootSize;
												break;
								case 4:			// UTILITIES
												if ( itemPressed < 0 || itemPressed > itemCount_util ) { previewReady = 0; break; }
												previewCategory		= catListUtilities[itemPressed].cat;
												strcpy( previewName, 		catListUtilities[itemPressed].name 			);
												strcpy( previewAuthor, 		catListUtilities[itemPressed].author 		);
												strcpy( previewVersion, 	catListUtilities[itemPressed].version 		);
												strcpy( previewDlSize, 		catListUtilities[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListUtilities[itemPressed].release 		);
												strcpy( previewDescription, catListUtilities[itemPressed].description 	);
												strcpy( previewLink, 		catListUtilities[itemPressed].link 			);
												strcpy( previewDir, 		catListUtilities[itemPressed].dir 			);
												previewEbootSize		= catListUtilities[itemPressed].ebootSize;
												break;
								case 5:			// THEMES
												if ( itemPressed < 0 || itemPressed > itemCount_themes ) { previewReady = 0; break; }
												previewCategory		= catListThemes[itemPressed].cat;
												strcpy( previewName, 		catListThemes[itemPressed].name 		);
												strcpy( previewAuthor, 		catListThemes[itemPressed].author 		);
												strcpy( previewVersion, 	catListThemes[itemPressed].version 		);
												strcpy( previewDlSize, 		catListThemes[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListThemes[itemPressed].release 		);
												strcpy( previewDescription, catListThemes[itemPressed].description 	);
												strcpy( previewLink, 		catListThemes[itemPressed].link 		);
												strcpy( previewDir, 		catListThemes[itemPressed].dir 			);
												previewEbootSize		= catListThemes[itemPressed].ebootSize;
												break;
								case 6:			// DEMOS
												if ( itemPressed < 0 || itemPressed > itemCount_demos ) { previewReady = 0; break; }
												previewCategory		= catListDemos[itemPressed].cat;
												strcpy( previewName, 		catListDemos[itemPressed].name 			);
												strcpy( previewAuthor, 		catListDemos[itemPressed].author 		);
												strcpy( previewVersion, 	catListDemos[itemPressed].version 		);
												strcpy( previewDlSize, 		catListDemos[itemPressed].dlSize 		);
												strcpy( previewRelease, 	catListDemos[itemPressed].release 		);
												strcpy( previewDescription, catListDemos[itemPressed].description 	);
												strcpy( previewLink, 		catListDemos[itemPressed].link 			);
												strcpy( previewDir, 		catListDemos[itemPressed].dir 			);
												previewEbootSize		= catListDemos[itemPressed].ebootSize;
												break;
								}
							if ( previewReady == 1 )
								{
								strtok_r( previewDescription, "|", &previewDesLine2 );
								strtok_r( previewDesLine2, "|", &previewDesLine3 );
								strtok_r( previewDesLine3, "|", &previewDesLine4 );
								strtok_r( previewDesLine4, "|", &previewDesLine5 );
								previewActive		=  1;
								previewListNumber	= itemPressed;
								// CHECK IF INSTALLED
								if ( access( previewDir, F_OK ) == -1 ) { preview_isInstalled = 0; }
								else  									{ preview_isInstalled = 1; }
								// CHECK IF DOWNLOADED
								if ( access( string_join( 3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk" ), F_OK ) == -1 ) { preview_isDownloaded = 0; }
								else 																							   { preview_isDownloaded = 1; }
								// CHECK IF LATEST VERSION
								//preview_isCurrent
								}
							itemPressed 		= -1;
							}
						}
					else
						{
						// DOWNLOAD/INSTALL/UPDATE PRESSED
						if ( point_in_rectangle( touch_x, touch_y, 197, 213, 357, 266 ) == 1 )
							{
							/// CHECK WHICH BUTTON IS SHOWING
								
								// DOWNLOAD
								strcpy( dialogMessage, string_join( 4, "Downloading\n", previewName, "\n", previewRelease ) );
								char *fileVpkCloud;
								char *fileVpkLocal;
								fileVpkCloud		= string_join( 3, VHBB_CLOUD_ADDRESS_FILES, previewName, ".vpk" );
								fileVpkLocal		= string_join( 3, VHBB_APP_ADDRESS_STORAGE_FILES, previewName, ".vpk" );
								
								if ( download_file( fileVpkCloud, fileVpkLocal ) < 0 )
									{
									// DOWNLOAD FAILED
									
									}
								else
									{
									
									}
								
								
								
							}
						if ( point_in_rectangle( touch_x, touch_y, 0, 460, 90, 544 ) == 1 && settingsDraw == 0 )
							{
							previewActive	= 0;
							itemPressed		= previewListNumber;
							}
						}
					}
				else
					{
					/// touch RELEASE FOR SETTINGS HERE
					}
				}
			else
				{
				if ( point_in_rectangle( touch_x, touch_y, 287, 403, 682, 448 ) == 1 )
					{
					dialogOpen		= 0;
					}
				
				
				}
				
			// RESET BUTTON STATES
			btnState_previewAction			= 0;
			btnState_settingsSound			= 0;
			btnState_settingsCtrls			= 0;
			btnState_settingsContact		= 0;
			btnState_settingsAbout			= 0;
			btnState_dialogBtn2				= 0;
			btnState_search 				= 0;
			clickable = 1;
			}
		//-------------------------------------------------------------
		//-------------------------------------------------------------
		/// MAIN DRAW -------------------------------------------------
		#include "draw_main.h"
		///------------------------------------------------------------
		
			
			
			
			
		
		}
		
		
	/// MAIN DRAW -------------------------------------------------
	#include "shutdown.h"
	///------------------------------------------------------------
	return 0;
	}