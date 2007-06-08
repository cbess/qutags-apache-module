/* C. Bess Creation (Quantum Quinn) - Created: Thu Jul 13 2006 - QuTags */
// modified 2006 Jul 21
// Copyright (C) 2006, 2007 Christopher Bess of Quantum Quinn
// - C. Bess Creation -<qutags@quantumquinn.com>
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_variables.h"
#include "qutags.h"

ZEND_DECLARE_MODULE_GLOBALS(qutags)

//#define QUPRC "_QURPC_"
#define QUID "_QUID_" // the id sent back
#define QUFUNC "_QUFUNC_" // the function to execute on the server (php script)

#include "quinclude.h"

static function_entry qutag_functions [ ] = 
{
	// init qutag engine
	ZEND_FE( init_qutags, NULL )
	
	// html output
	ZEND_FE( quform_start, NULL )
	ZEND_FE( quform_end, NULL )
	ZEND_FE( qubutton, NULL )
	ZEND_FE( qutextbox, NULL )
	ZEND_FE( qudiv, NULL )
	ZEND_FE( qudiv_start, NULL )
	ZEND_FE( qudiv_end, NULL )
	ZEND_FE( qulistbox, NULL )
	ZEND_FE( qulistbox_start, NULL )
	ZEND_FE( qulistbox_end, NULL )
	ZEND_FE( quoption, NULL )
	ZEND_FE( qutextarea, NULL )
	ZEND_FE( qutextarea_start, NULL )
	ZEND_FE( qutextarea_end, NULL )
	ZEND_FE( quhyperlink, NULL )
	ZEND_FE( quradiobutton, NULL )
	ZEND_FE( qucheckbox, NULL )
	ZEND_FE( qufloatingdiv_start, NULL )
	ZEND_FE( qufloatingdiv_end, NULL )
	ZEND_FE( quimage, NULL )
	
	// misc
	ZEND_FE( qustatus, NULL )
	ZEND_FE( qu_add_option, NULL )
	ZEND_FE( qu_clear_options, NULL )
	ZEND_FE( qutimer, NULL )
	
	// output helpers
	ZEND_FE( qu_echo, NULL )
//	ZEND_FE( qu_return, NULL )
	ZEND_FE( qu_repeat, NULL )
	ZEND_FE( qu_repeat_kv, NULL )
	
	// general responses
	ZEND_FE( quresponse, NULL )
	
	// callback specific responses
	ZEND_FE( send_quvalue, NULL )
	ZEND_FE( send_quinnerhtml, NULL )
	ZEND_FE( send_qudisplay, NULL )
	ZEND_FE( send_qudisabled, NULL )
	ZEND_FE( send_jscode, NULL )
	
	// nothing
	{ NULL, NULL, NULL }
};

zend_module_entry qutags_module_entry =
{
	STANDARD_MODULE_HEADER,
	"QuTags",
	qutag_functions,
	ZEND_MINIT(minit_qutags), 
	ZEND_MSHUTDOWN(mshutdown_qutags), 
	ZEND_RINIT(rinit_qutags),
	ZEND_RSHUTDOWN(rshutdown_qutags),
	ZEND_MINFO( minfo_qutags ),
	"0.7.1", // module version
	STANDARD_MODULE_PROPERTIES
};

#if COMPILE_DL_QUTAGS
ZEND_GET_MODULE( qutags )
#endif

// function implementations (below)
#include "qutags_mr_functions.h" // module & request function (cycle)
#include "qutags_tags.h" // qu_button, qu_div, etc
#include "qutags_functions.h" // qu_response, send_quvalue, etc | includes utility functions

ZEND_FUNCTION( init_qutags )
{			
	// get the number of args passed to this func (should be one)
	int numArgs = ZEND_NUM_ARGS();	
	int postReq = TRUE; // 1 means don't change default request type	
	char *qucommLoc = (char*)NULL;
	char *securekey = (char*)NULL; // the secure key seed
	char *reqType = NULL;
	
	if (numArgs == 1)
	{
		zval *tagArr = NULL;
		zval **arrVal = NULL;
		
		// get one zval (assoc array)
		if ( zend_parse_parameters( 1 TSRMLS_DC,
		"z", &tagArr ) != SUCCESS )
		{
			OUTPUT( "failed parse parameters. parameter must be ([ASSOC_ARRAY])" );
		} // end IF

		// get the location of the qucomm js	
		AARRAY_SVALUE("qucomm", qucommLoc);
		
		// get secure key info from user
		AARRAY_SVALUE("securekey", securekey);
		
		// --> get request type
		AARRAY_SVALUE("request", reqType);
		
		if ( strlen(reqType) )
		{
			if ( !strcasecmp(reqType, "get") )
				postReq = FALSE;			
		} // end IF
		// <-- end
			
	} // end IF
	else if ( numArgs > 1 )
	{ // passed to many parameters
		OUTPUT( "pass 1 (one) parameter ([ASSOC_ARRAY])" );
	} // end ELSE-IF
	
	// declare the hash table to hold the request collection
	HashTable *hash = NULL;
	
	//	Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_GET])
/*
PG(http_globals)[TRACK_VARS_SERVER]
PG(http_globals)[TRACK_VARS_ENV]
PG(http_globals)[TRACK_VARS_GET]
*/	
	// based on request type choose GET or POST hashtable
	if ( postReq )
		hash = Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_POST]);
	else
		hash = Z_ARRVAL_P(PG(http_globals)[TRACK_VARS_GET]);

	zval **idVal = NULL;
	zval **funcVal = NULL;
	
	// get values from $_post array
	if  ( zend_hash_find(
	hash, // search POST array
	QUID, strlen(QUID)+1, 
	(void**)&idVal) == SUCCESS )
	{	
		// set to ajax mode
		// tells all tags to output js
		set_iscallback( 1 );
	
		char *userFunc = (char*)NULL;
		
		// get QUMETHOD
		if  ( zend_hash_find(
		hash, // search POST array
		QUFUNC, strlen(QUFUNC)+1, 
		(void**)&funcVal) == SUCCESS )
		{	
			userFunc = estrdup( Z_STRVAL_PP(funcVal) );
			
			// passing back the ID/NAME that invoked the callback
			zval **params[1];
			params[0] = idVal;
			if ( call_function_ex( userFunc, 1, params ) == NULL )
			{
				ECHO("cannot find function: %s", userFunc);
			} // end IF
			
			efree(userFunc);
			zval_ptr_dtor(params);
			DTOR(funcVal);
		} // end IF
				
		// clean up
		DTOR(idVal);
		
		// exit script
		zend_bailout();
		
	} // end IF
	else
	{
		// setup qucomm loc format
		int nQucommLoc = strlen(qucommLoc);
		char *qucommf = estrdup("<script src='%squcomm.js'></script>");
		
		// allocate space
		char *qucomm = (char*)emalloc(nQucommLoc+strlen(qucommf)+1);
		
		// create the js include tag
		sprintf(qucomm, qucommf, (nQucommLoc==0 ? "" : qucommLoc) );
			
		// print to the header buffer
		PUTS_H(qucomm);
	} // end ELSE
}
