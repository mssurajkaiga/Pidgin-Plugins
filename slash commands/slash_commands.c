/*
 * Selective Logging Plugin
 *
 * Copyright (C) 2012, M S SURAJ <mssurajkaiga@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */
 
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* Include glib.h for various types */
#include <glib.h>

#ifndef G_GNUC_NULL_TERMINATED
# if __GNUC__ >= 4
#  define G_GNUC_NULL_TERMINATED __attribute__((__sentinel__))
# else
#  define G_GNUC_NULL_TERMINATED
# endif
#endif

/* This is the required definition of PURPLE_PLUGINS as required for a plugin,
 * but we protect it with an #ifndef because config.h may define it for us
 * already and this would cause an unneeded compiler warning. */
 
#ifndef PURPLE_PLUGINS
# define PURPLE_PLUGINS
#endif

#include "account.h"
#include "cmds.h"
#include "plugin.h"
#include "savedstatuses.h"
#include "status.h"
#include "version.h"

#define PLUGIN_ID "core-mssurajkaiga-slashcommands"
#define PLUGIN_AUTHOR "M S SURAJ <mssurajkaiga@gmail.com>"

static PurplePlugin *slash_commands = NULL;

static PurpleCmdId away_command_id, dnd_command_id, online_command_id; 
/**
 * Used to hold a handle to the commands we register. Holding this handle
 * allows us to unregister the commands at a later time. 
 */

static PurpleCmdRet 
away_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data) /* away command call-back function */
{

PurpleSavedStatus *away = NULL;
  
  if(args[0]){
  
	  if(purple_savedstatus_get_type(purple_savedstatus_find(args[0]))==PURPLE_STATUS_AWAY){
		away = purple_savedstatus_find(args[0]);
		}
		
	  else   {
	  	away = purple_savedstatus_new(args[0], PURPLE_STATUS_AWAY);
	  	}
	  	
	}
	
  else {
	  away = purple_savedstatus_find("Away");
  
	  if(away){
	  	 if(purple_savedstatus_get_type(away)!=PURPLE_STATUS_AWAY){
	  	 	away = purple_savedstatus_new(NULL, PURPLE_STATUS_AWAY);
  			}
  		}

  	  else away = purple_savedstatus_new(NULL, PURPLE_STATUS_AWAY);
  
  	  	}
  

  purple_savedstatus_activate(away);
  return PURPLE_CMD_RET_OK;

}

static PurpleCmdRet 
dnd_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data) /* dnd command call-back function */
{
  PurpleSavedStatus *dnd = NULL;
  
  if(purple_savedstatus_is_idleaway()){
  	purple_savedstatus_set_idleaway(FALSE);
  }
  
  if(args[0]){
  
	  if(purple_savedstatus_get_type(purple_savedstatus_find(args[0]))==PURPLE_STATUS_UNAVAILABLE){
		dnd = purple_savedstatus_find(args[0]);
		}
		
	  else   {
	  	dnd = purple_savedstatus_new(args[0], PURPLE_STATUS_UNAVAILABLE);
	  	}
	  	
	}
	
  else {
	  dnd = purple_savedstatus_find("Do Not Disturb");
  
	  if(dnd){
	  	 if(purple_savedstatus_get_type(dnd)!=PURPLE_STATUS_UNAVAILABLE){
	  	 	dnd = purple_savedstatus_new(NULL, PURPLE_STATUS_UNAVAILABLE);
  			}
  		}

  	  else dnd = purple_savedstatus_new(NULL, PURPLE_STATUS_UNAVAILABLE);
  
  	  	}
  

  purple_savedstatus_activate(dnd);
  return PURPLE_CMD_RET_OK;

}

static PurpleCmdRet 
online_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data) /* online command call-back function */
{

PurpleSavedStatus *online = NULL; /* creates a new saved status pointer*/

  if(purple_savedstatus_is_idleaway()){   /* checks if current status is idle-away*/
  	purple_savedstatus_set_idleaway(FALSE);  /* deactivates the idle-away status - not necessary but sometimes problem occurs*/
  }  
  
  if(args[0]){  /* checks if arguments are passed in the comment*/
  
	  if(purple_savedstatus_get_type(purple_savedstatus_find(args[0]))==PURPLE_STATUS_AVAILABLE){ /*checks if status exists */
		online = purple_savedstatus_find(args[0]); /*assigns the existing status to the pointer */
		} 
		
	  else   {
	  	online = purple_savedstatus_new(args[0], PURPLE_STATUS_AVAILABLE); /* creates new saved status and assigns it to the pointer */
	  	}
	  	
	}
	
  else {
	  online = purple_savedstatus_find("Available"); /* finds default saved status with the given title */
  
	  if(online){
	  	 if(purple_savedstatus_get_type(online)!=PURPLE_STATUS_AVAILABLE){
	  	 	online = purple_savedstatus_new(NULL, PURPLE_STATUS_AVAILABLE); /* if status is not of required type, create new*/
  			}
  		}

  	  else online = purple_savedstatus_new(NULL, PURPLE_STATUS_AVAILABLE); /* if status doesn't exist, create new */
  
  	  	}
  

  purple_savedstatus_activate(online); /*activates the saved status */
  return PURPLE_CMD_RET_OK; /* return if command executed successfully */

}


static gboolean
plugin_load(PurplePlugin *plugin)
{
	gchar *away_help =NULL; /* Declare all vars up front. Avoids warnings on some compilers */
	gchar *dnd_help =NULL;
	gchar *online_help =NULL;
	
	slash_commands = plugin;  /* Save a handle to ourself for later */
	
	away_help = "away &lt;title&gt;:  Sets your status as away with the title(optional).";
	dnd_help = "dnd &lt;title&gt;:  Sets your status as unvailable with the title(optional).";
	online_help = "online &lt;title&gt;:  Sets your status as available with the title(optional)";
	
	away_command_id = purple_cmd_register 
    ("away",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */
     PLUGIN_ID,                     /* Plugin ID */
     away_cb,                        /* Name of the callback function */
     away_help,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
    dnd_command_id = purple_cmd_register 
    ("dnd",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */ 
     PLUGIN_ID,                     /* Plugin ID */
     dnd_cb,                        /* Name of the callback function */
     dnd_help,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
    online_command_id = purple_cmd_register 
    ("online",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */
     PLUGIN_ID,                     /* Plugin ID */
     online_cb,                        /* Name of the callback function */
     online_help,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
     
    /* Just return true to tell libpurple to finish loading. */ 
    return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{

  purple_cmd_unregister(away_command_id); /* unregister the away command */
  purple_cmd_unregister(dnd_command_id); /* unregister the dnd command */
  purple_cmd_unregister(online_command_id); /* unregister the online command */

  /* Just return true to tell libpurple to finish unloading. */
  return TRUE;
}

static PurplePluginInfo info = {
        PURPLE_PLUGIN_MAGIC,        						/* magic number */
        PURPLE_MAJOR_VERSION,       						/* purple major */
        PURPLE_MINOR_VERSION,      							/* purple minor */
        PURPLE_PLUGIN_STANDARD,     						/* plugin type */
        NULL,                       						/* UI requirement */
        0,                          						/* flags */
        NULL,                       						/* dependencies */
        PURPLE_PRIORITY_DEFAULT,    						/* priority */

        PLUGIN_ID,                  						/* id */
        "Slash Commands",        							/* name */
        "1.0",                      						/* version */
        "Enables slash commands to alter your status",    	/* summary */
        "/away <message>, /online <message>, /dnd <message>, <message> is optional",  	/* description */
        PLUGIN_AUTHOR,              						/* author */
        "http://mssuraj.wordpress.com",         			/* homepage */

        plugin_load,                						/* load */
        plugin_unload,              						/* unload */
        NULL,                       						/* destroy */

        NULL,                       						/* ui info */
        NULL,                       						/* extra info */
        NULL,                       						/* prefs info */
        NULL,                       						/* actions */
        NULL,                       						/* reserved */
        NULL,                       						/* reserved */
        NULL,                       						/* reserved */
        NULL                        						/* reserved */
};

static void
init_plugin(PurplePlugin *plugin)
{
}

PURPLE_INIT_PLUGIN(slash_commands, init_plugin, info)
