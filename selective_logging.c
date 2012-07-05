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
#include "conversation.h"
#include "log.h"
#include "plugin.h"
#include "status.h"
#include "version.h"

#define PLUGIN_ID "core-mssurajkaiga-selectivelogging"
#define PLUGIN_AUTHOR "M S SURAJ <mssurajkaiga@gmail.com>"

PurpleLogLogger *logger = NULL;
PurpleStatus *stat = NULL;
GList *list = NULL;

static void select_logging(PurpleConversation *conv)
{
	if(stat){
	
		if(purple_status_is_available(stat)){
				if(!purple_conversation_is_logging(conv)){
					purple_conversation_set_logging(conv, TRUE); /* enables logging if status is available */
				}	
		}
		
		else{
				if(purple_conversation_is_logging(conv)){
					purple_conversation_set_logging(conv, FALSE); /* disables logging if status is not available */
			}
		}
	}
}

static void reset(PurpleConversation *conv){
	purple_conversation_set_logging(conv,TRUE); /* enables logging for the conversation */
}

static void status_changed(PurpleAccount *account, PurpleStatus *status){

	stat = status;	
	
	list = purple_get_conversations(); /* gets list of all conversations open*/
	
	g_list_foreach(list,(GFunc)select_logging,NULL); /* decides if logging is to be enabled/disabled for the conversations */
}

static PurpleAccountUiOps account_uiops = {
	NULL,             /* notify_added          */
	status_changed,   /* status_changed        */
	NULL,             /* request_add           */
	NULL,             /* request_authorize     */
	NULL,             /* close_account_request */
	NULL,             /* _purple_reserved1     */
	NULL,             /* _purple_reserved1     */
	NULL,             /* _purple_reserved1     */
	NULL              /* _purple_reserved1     */
};
  
static gboolean
plugin_load(PurplePlugin *plugin)
{
  
  list = purple_get_conversations();  /* gets list of all conversations open*/

  g_list_foreach(list,(GFunc)select_logging,NULL); /* decides if logging is to be enabled/disabled for the conversations */

  purple_accounts_set_ui_ops(&account_uiops); 
  
  /* Just return true to tell libpurple to finish loading. */
  return TRUE;
}

static gboolean
plugin_unload(PurplePlugin *plugin)
{
  list = purple_get_conversations();
	
  g_list_foreach(list,(GFunc)reset,NULL);	/* logging is enabled for all the conversations */
  
  /* Just return true to tell libpurple to finish unloading. */
  return TRUE;
}

/**
 * Struct used to let Pidgin understand our plugin
 */
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
        "Selective Logging",        						/* name */
        "1.0",                      						/* version */
        "Disables/enables logging based on user status",    /* summary */
        "Logs only if your status is available",      		/* description */
        PLUGIN_AUTHOR,              						/* author */
        "mssuraj.wordpress.com",         					/* homepage */

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

PURPLE_INIT_PLUGIN(selective_logging, init_plugin, info)
