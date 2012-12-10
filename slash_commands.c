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

static PurpleCmdRet 
away_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data)
{

  /*if(args[0]){
  	purple_savedstatus_set_message(purple_savedstatus_get_idleaway(),args[0]);
  }
  
  purple_savedstatus_set_idleaway(TRUE);

  return PURPLE_CMD_RET_OK;
	*/

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
dnd_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data)
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
online_cb(PurpleConversation *conv, const gchar *cmd, gchar **args, gchar **error, void *data)
{
  
  /*if(args[0]){
  	purple_savedstatus_set_message(purple_savedstatus_get_default(),args[0]);
  }
  purple_savedstatus_set_idleaway(FALSE);

  return PURPLE_CMD_RET_OK;
*/

PurpleSavedStatus *online = NULL;

  if(purple_savedstatus_is_idleaway()){
  	purple_savedstatus_set_idleaway(FALSE);
  }  
  
  if(args[0]){
  
	  if(purple_savedstatus_get_type(purple_savedstatus_find(args[0]))==PURPLE_STATUS_AVAILABLE){
		online = purple_savedstatus_find(args[0]);
		}
		
	  else   {
	  	online = purple_savedstatus_new(args[0], PURPLE_STATUS_AVAILABLE);
	  	}
	  	
	}
	
  else {
	  online = purple_savedstatus_find("Available");
  
	  if(online){
	  	 if(purple_savedstatus_get_type(online)!=PURPLE_STATUS_AVAILABLE){
	  	 	online = purple_savedstatus_new(NULL, PURPLE_STATUS_AVAILABLE);
  			}
  		}

  	  else online = purple_savedstatus_new(NULL, PURPLE_STATUS_AVAILABLE);
  
  	  	}
  

  purple_savedstatus_activate(online);
  return PURPLE_CMD_RET_OK;

}


static gboolean
plugin_load(PurplePlugin *plugin)
{
	slash_commands = plugin;
	
	away_command_id = purple_cmd_register 
    ("away",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */
     PLUGIN_ID,                     /* Plugin ID */
     away_cb,                        /* Name of the callback function */
     NULL,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
    dnd_command_id = purple_cmd_register 
    ("dnd",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */ 
     PLUGIN_ID,                     /* Plugin ID */
     dnd_cb,                        /* Name of the callback function */
     NULL,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
    online_command_id = purple_cmd_register 
    ("online",                         /* command name */ 
     "s",                           /* command argument format */
     PURPLE_CMD_P_DEFAULT,          /* command priority flags */  
     PURPLE_CMD_FLAG_IM | PURPLE_CMD_FLAG_ALLOW_WRONG_ARGS,            /* command usage flags */
     PLUGIN_ID,                     /* Plugin ID */
     online_cb,                        /* Name of the callback function */
     NULL,                      /* Help message */
     NULL                           /* Any special user-defined data */
     );
     
     
    /* Just return true to tell libpurple to finish loading. */ 
    return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{

  purple_cmd_unregister(away_command_id);
  purple_cmd_unregister(dnd_command_id);
  purple_cmd_unregister(online_command_id);

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
        "Pre-defined slash commands alter your status",  	/* description */
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
