/****************************************************************************************//*                                      SOUNDS.CP                                       *//****************************************************************************************//*          (c) 1994 by Magnet Interactive Studios, inc. All rights reserved.           *//****************************************************************************************//*  Revision History:                                                                   *//*          7/20/94   File first created, by Andrew Looney. These routines were pulled  *//*                    right out of Icebreaker.c, and were written by Mike Chaney.       *//*  v1.0     8/3/94   First polished version. By Andrew Looney.                         *//*  v2.0    8/24/94   Seven level version burned onto CD-ROM. By Andrew Looney.         *//*  v2.1    8/29/94   Comments brought back up to date. By Andrew Looney.               *//*  v2.2     9/7/94   Sixteen level version. By Andrew Looney.                          *//*  v2.3    9/23/94   Thirty level version burned onto CD-ROM. By Andrew Looney.        *//*  v2.4   10/19/94   Fifty level version. By Andrew Looney.                            *//*  v2.5   10/31/94   Sixty level version burned onto CD-ROM. By Andrew Looney.         *//*  v3.0   10/31/94   Began switching over to real artwork. By Andrew Looney.           *//*  v3.1   11/11/94   COMDEX version. By Andrew Looney.                                 *//*  v3.2    12/2/94   Seventy-five level version. By Andrew Looney.                     *//*  v3.3   12/14/94   Ack! Zombies! By Andrew Looney.                                   *//*  v3.4   12/23/94   Orange Meanies! By Andrew Looney.                                 *//*  v3.5     1/3/95   100 Levels! By Andrew Looney.                                     *//*  v3.6     1/5/95   101 Levels. By Andrew Looney.                                     *//*  v3.7    1/16/95   New, improved Brainy Seeker logic! By Andrew Looney.              *//*  v3.8    1/27/95   114 level version burned onto CD-ROM. By Andrew Looney.           *//*  v3.9     2/3/95   New CD-ROM burned for shipment to 3DO. By Andrew Looney.          *//*  v4.0     2/9/95   New CD-ROM featuring the big level grid. By Andrew Looney.        *//*  v4.1    2/22/95   New CD-ROM with rough draft of full interface. By Andrew Looney.  *//*  v4.2    3/17/95   The St. Patrick's Day Version. By Andrew Looney.                  *//*  v4.3    3/20/95   The last version before the movies get added. By Andrew Looney.   *//*  v4.4    3/21/95   First version with movies integrated. By Andrew Looney.           *//*          3/22/95   Altered to enable dynamic loading of sound fx. By Mark Emon.      *//*  v4.5    3/22/95   Second version with movies integrated. By Andrew Looney.          *//*  v4.6    3/27/95   Third version with movies integrated. By Andrew Looney.           *//*  v5.0    3/28/95   Newest version sent to QA. By Andrew Looney.                      *//*  v5.1    3/28/95   Now, with Easter Eggs! By Andrew Looney.                          *//*  v5.2    3/29/95   Could this be the final version? By Andrew Looney.                *//*  v5.3    3/30/95   OK, now maybe THIS is the final version! By Andrew Looney.        *//*  v5.4     4/3/95   Made a couple more minor changes. By Andrew Looney.               *//****************************************************************************************//***************************** WHAT THIS SOFTWARE DOES **********************************   These routines provide an easy way of accessing a large group of sound effects at thesame time, by using a MIDI score. The sounds are loaded dynamically as needed and can then be accessed by their index numbers.*****************************************************************************************//***** includes (make sure CPlusSwiHack.h is the last one) *****/#include "graphics.h"#include "stdio.h"#include "stdlib.h"#include "mem.h"#include "types.h"#include "hardware.h"#include "event.h"#include "strings.h"#include "access.h"#include "UMemory.h"#include "Form3DO.h"#include "Init3DO.h"#include "Parse3DO.h"#include "Utils3DO.h"#include "audio.h"#include "music.h"/***** Magnet includes *****/#include "sounds.h"/***** special c++ include (this must be last) *****/#include "CPlusSwiHack.h"/***** global variables *****/extern ScoreContext   *g_scon;extern bool	         sound_on;typedef struct{	bool SoundEffectsNowLoaded;	Item Sample;	Item Attachment;} soundfx;soundfx  fx[MAXPROGRAMNUM];/****** extern variables ******/extern bool SoundfxNeeded[];  /******************************  pmfSetupScoreContext  **********************************   This function creates a ScoreContext, PIMap information will be loaded dynamically *****************************************************************************************/ScoreContext* pmfSetupScoreContext( )  // I need to get rid of this mapfile thing{	ScoreContext *scon;	int32 Result;	/* Create a context for interpreting a MIDI score and tracking notes. */	scon = CreateScoreContext( MAXPROGRAMNUM );	if( scon == (ScoreContext *) NULL )	{		return ((ScoreContext *) NULL);	}/* Specify a mixer to use for the score voices. */	Result = InitScoreMixer( scon, MIXER_NAME, MAX_SCORE_VOICES, MIXER_AMPLITUDE );/* Load Instrument Templates from disk and fill Program Instrument Map. *//* All instruments were loaded using LoadPIMap() but  As an alternative, *//* I have used SetPIMapEntry() loadinstrument to load instruments dynamically */		return (scon);}/******************************  DynamicSampleLoader  **********************************  This function dynamically loads and unloads PIMap information and sampled sound effects   by comparing what new sound effects are needed to what sound effects are presently loaded *****************************************************************************************/void DynamicSampleLoader( bool SoundfxNeeded[] ){	Item Instrument;	Err Zebra;		for( int i = 0; i < MAXPROGRAMNUM; i++)	{		if ( SoundfxNeeded[i])		{			if (!fx[i].SoundEffectsNowLoaded)  //if a sound fx is not here and needed let's load it!			{				Instrument =  LoadInsTemplate( "halfmono8.dsp", 0 );					if (!Instrument)						{							printf("the Instrument template did not load up\n");						}												fx[i].Sample = LoadSample( SoundEffectsNames[i] ); // need an array of these						if (!fx[i].Sample)						{								printf("LoadSample did not load up\n");						}											fx[i].Attachment = AttachSample( Instrument, fx[i].Sample,  (char *)NULL );						if (!fx[i].Attachment)						{							printf("AttachSample did not load up\n");						}																	Zebra = SetPIMapEntry( g_scon , i ,  Instrument, 4, 100 );						if (Zebra < 0)						{							printf("SetPIMapEntry did not load up  \n");						}													fx[i].SoundEffectsNowLoaded = TRUE;								}		}		else  // Sound effect must not be needed so let's unload it if it is present		{			if (fx[i].SoundEffectsNowLoaded)			{				DetachSample(fx[i].Attachment);				UnloadSample(fx[i].Sample);				FreeChannelInstruments( g_scon, i%16);				ScavengeMem();				fx[i].SoundEffectsNowLoaded = FALSE;							}		}	ScavengeMem();	}}/*****************************  pmfCleanupScoreContext  *********************************   This function unloads the PIMap, which frees the instruments and samples, thendeletes the ScoreContext.*****************************************************************************************/Err pmfCleanupScoreContext( ScoreContext *scon ){	if( scon != NULL )	{		for(int i = 0; i < MAXPROGRAMNUM; i++)		{			SoundfxNeeded[i] = FALSE; // we no longer need any sound effects		}			DynamicSampleLoader( SoundfxNeeded );		TermScoreMixer( scon );		DeleteScoreContext( scon );	}		return (0);}/***********************************  InitAudio  ****************************************   This function handles all aspects of initializing the audio for the 3DO, includingmaking the call to pmfSetupScoreContext that reads in and interprets the PIMap file.*****************************************************************************************/bool InitAudio ( ){	/* Initialize audio, return if error. */	if (OpenAudioFolio())	{		printf("InitAudio: Audio Folio could not be opened!\n");		return(FALSE);	}	g_scon = pmfSetupScoreContext();	if (g_scon == NULL)	{		pmfCleanupScoreContext(g_scon);		CloseAudioFolio();		printf ("InitAudio: Could not initialize sound effects\n");		return(FALSE);	}	// initialize array to FALSE  no sound effects loaded yet	for( int x = 0; x < MAXPROGRAMNUM; x++)	{		fx[x].SoundEffectsNowLoaded = FALSE;	}			/*********************************************************************** 	* SoundfxNeeded are pre-set in sounds.h for the sound effects that are *	* universally utilized. Other sound effects will be loaded dynamically *	***********************************************************************/		DynamicSampleLoader( SoundfxNeeded );		return(TRUE);}/*********************************  ReleaseAudio  ***************************************   This function closes down the audio elements and frees up the resources we were using.*****************************************************************************************/void ReleaseAudio(){	int32 i;		for (i = 0; i < MAXPROGRAMNUM; i++)			{		if(fx[i].SoundEffectsNowLoaded)			EndSoundEffect(i);	}	pmfCleanupScoreContext(g_scon);	CloseAudioFolio();}/********************************  PlaySoundEffect  *************************************	Given an index into the list of sound effects specified in the PIMap file, thisfunction starts playing the specified sound effect.*****************************************************************************************/void PlaySoundEffect(int32 sound_id){	int32 Result;	int32 curChannel;		if (!(sound_on))		return;	curChannel=(sound_id%16);	ChangeScoreProgram( g_scon, curChannel, sound_id );	Result=StartScoreNote( g_scon, curChannel, 60, 100 );	if (Result < 0)	{		printf("PlaySoundEffect: Warning: Could not play sound #%ld.\n",sound_id);	}		Result=ReleaseScoreNote( g_scon, curChannel, 60, 100 );	if (Result < 0)	{		printf("PlaySoundEffect: Warning: Could not stop sound #%ld.\n",sound_id);	}}/*********************************  EndSoundEffect  *************************************	Given an index into the list of sound effects specified in the PIMap file, thisfunction halts the specified sound effect.*****************************************************************************************/void EndSoundEffect(int32 sound_id){	int32 Result;	int32 curChannel;	curChannel=(sound_id%16);	ChangeScoreProgram( g_scon, curChannel, sound_id );	Result=ReleaseScoreNote( g_scon, curChannel, 60, 100 );	if (Result < 0)	{		printf("EndSoundEffect: Warning: Could not stop sound #%ld.\n",sound_id);	}}/************************************* EOF **********************************************/