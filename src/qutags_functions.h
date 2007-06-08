// misc functions
/*
	created: july 30, 2006
	- used to hold all the exposed qutag helper/utility functions
	updated: july 30, 2006
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
*/

// duplicates its argument's string value
ZEND_FUNCTION( qu_echo )
{
	
	char *value;
	int value_len;
	int repeat; // how many times to echo the value
	
	if ( ZEND_NUM_ARGS() != 2 )
		OUTPUT( "pass 2 (two) parameters (STRING, NUM)" );
	
	if ( zend_parse_parameters(2 TSRMLS_DC,
		"sl", &value, &value_len, &repeat ) == FAILURE )
	{
		OUTPUT( "failed parse parameters." );
	} // end IF
			
	if ( repeat <= 0 )
		OUTPUT( "Repeat parameter invalid." );
		
	do {	
	PHPWRITE( value, value_len );
	} while( (--repeat) != 0 );		
}

/**
* same as qu_echo, except it returns a string
*/
ZEND_FUNCTION( qu_return )
{
	return;
	
	char *value;
	int value_len;
	int repeat; // how many times to echo the value
	
	if ( ZEND_NUM_ARGS() != 2 )
		OUTPUT( "pass 2 (two) parameters (STRING, NUM)" );
	
	if ( zend_parse_parameters(2 TSRMLS_DC,
		"sl", &value, &value_len, &repeat ) == FAILURE )
	{
		OUTPUT( "failed parse parameters." );
	} // end IF
			
	if ( repeat <= 0 )
		OUTPUT( "Repeat parameter invalid." );
	
	// free the needed memory
	char *out = NULL;
	out = (char*)emalloc( (value_len*repeat)+2 );
	out = "";
	
	if ( out )
	{
		// concatenates the string repeat times	
		do {	
			strcat( out, value );
		} while( (--repeat) != 0 );
	
		RETURN_STRING( out, 0 );
	} // end IF
} // end

ZEND_FUNCTION( quresponse )
{
	INIT_QUTAG_ARGS
	
	IF_CALLBACK
		
	// --> send values via quresponse
	char *tagVal;
	SEND_quVALUE(tagVal);
		
	char *display;
	SEND_quDISPLAY(display);
		
	char *innerhtml;
	SEND_quINNER_HTML(innerhtml);
	
	char *css;
	SEND_quCLASS(css);
	
	char *disabled;
	SEND_quDISABLED(disabled);
	// <-- end
	
	END_CALLBACK
	
	DTOR(arrVal);
}

ZEND_FUNCTION( send_quvalue )
{	
	IF_CALLBACK
		
	SEND_QURESPONSE( JS_VALUE );
	
	END_CALLBACK
}

ZEND_FUNCTION( send_quinnerhtml )
{
	IF_CALLBACK
	
	SEND_QURESPONSE( JS_INNER_HTML );
	
	END_CALLBACK
}

ZEND_FUNCTION( send_qudisplay )
{
	IF_CALLBACK
	
	SEND_QURESPONSE( JS_DISPLAY );
	
	END_CALLBACK
} // end

ZEND_FUNCTION( send_jscode )
{
	char *jscode = NULL;
	if ( get_string_parameter(&jscode) == FAILURE )
	{
		OUTPUT( "pass 1 (one) parameter (JS Code)" );
	} // end IF
	
	IF_CALLBACK
		PUTS( js_response("JS", JS_CODE, jscode) );
	END_CALLBACK
}

/*
	- used to provide manipulation of the ajax status message
	- users can display there own (by providing an ID or use the default)
*/
ZEND_FUNCTION( qustatus )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
	// get the loading status message
	char *msg;
	AARRAY_SVALUE("msg", msg);	
		
	// get is centered value
	char *iscentered;
	AARRAY_SVALUE("centered", iscentered);
	
	// if the users want to use the default
	int useDefault = (strlen(tagId) == 0);
	
	if ( useDefault )
	{
		// assign default value
		if ( !strlen(msg) )
			msg = estrdup("Loading QuResponse...");
		
		// centered not null
		if ( strlen(iscentered) )
	 		ECHO( "<script>_isCentered=%s;</script>", iscentered );
	
		PUTS(  "<script>_qustatus='__qustatus';</script>" );
		ECHO( 
		"<div style='text-align:center;width:200px;height:15px;"
		"background-color:blue;color:white;font-size:16px;"
		"visibility:hidden;position:absolute;"
		"padding:2px;border:2px ridge silver;' id='__qustatus'>"
		"&nbsp;%s&nbsp;</div>\n", msg );
	}
	else
	{
		ECHO( "<script>_qustatus='%s';</script>", tagId );
	} // end ELSE
	
	END_CALLBACK
	
	DTOR(arrVal);
}

/*
	used to add a listbox option
*/
ZEND_FUNCTION( qu_add_option )
{
	// new Option(text, value, defaultSelected, selected)
	
	IF_CALLBACK
		char *tagId = NULL;
		char *text = NULL;
		char *value = NULL;
		int tagId_len;
		int text_len;
		int value_len;
		int numargs = ZEND_NUM_ARGS();
		
		// 1.) tagId / 2-?.) other parameters (tagId, text, value)
		if ( numargs < 2 )
			OUTPUT( "pass 2 (two) parameters (NAME, TEXT [, VALUE])" );

		if ( zend_parse_parameters(numargs TSRMLS_DC,
			"ss|s", &tagId, &tagId_len, &text, &text_len, &value, &value_len ) == FAILURE )
		{
			OUTPUT( "failed parse parameters." );
		} // end IF
				
		// 'JS_OPTION@tagId'=>new Array( 'text', 'value' )
		ECHO( 
		"'"JS_OPTION"@%s':['%s','%s'],", 
		tagId, text,
		(numargs > 2 ? value : "") );
		
	END_CALLBACK
}

/**
* - used to clear the options of a target listbox
*/
ZEND_FUNCTION( qu_clear_options )
{
	char *tagId = NULL;
	if ( get_string_parameter(&tagId) == FAILURE )
	{
		OUTPUT( "pass 1 (one) parameter (NAME)" );
	} // end IF
	
	IF_CALLBACK
		PUTS( js_response(tagId, JS_OPTION, "CLEAR") );
	END_CALLBACK
}

/** Aug 12, 2006
* - used to create a dynamic repeater that will use assoc arrays, the array count as an implicit multiplier
* and the value as the string to inject in the printf output (walks the array to output final string)
*/
ZEND_FUNCTION( qu_repeat )
{
	zval *userArr = NULL;

	int numArgs = ZEND_NUM_ARGS();
	
	// string format
	char *formatStr = (char*)NULL;
	int formatStr_len = 0;
	
	if ( numArgs != 2 )
	{
		OUTPUT( "pass 2 (two) parameters (STRING_FORMAT, ARRAY)." );
	} // end IF

	// get parameters
	if ( zend_parse_parameters(numArgs TSRMLS_CC,
		"sa", &formatStr, &formatStr_len, &userArr )
		== FAILURE )
	{
		OUTPUT( "failed parse parameters." );
	} // end IF
	
	// declare internal assoc array from user array
	HashTable *hash = Z_ARRVAL_P(userArr);
	HashPosition position;
	
 	// key's value
	zval **arrVal = NULL;
	
	// final returned string
	char *finalStr = (char*)NULL;
	finalStr = "";
	
	// loop through all the keys & values
	for( zend_hash_internal_pointer_reset_ex(hash, &position) ; 
	zend_hash_get_current_data_ex(hash, (void**)&arrVal, &position) == SUCCESS ; 
	zend_hash_move_forward_ex(hash, &position) ) 
	{		
		// declare need variables
		char *key = (char*)NULL;
		int key_len = 0;
		long index = 0;
		
		// gets key type
		int result = zend_hash_get_current_key_ex(hash, &key, &key_len, &index, 0, &position);
			
		// if the key is a string or an index (assoc-array or index-array)
		if ( (result == HASH_KEY_IS_STRING) || (result == HASH_KEY_IS_LONG) )
		{		
			// assign key's value to char variable
			char *value = estrdup(Z_STRVAL_PP(arrVal));
			
			// free mem for new val
			char *formatVal = (char*)emalloc( formatStr_len+strlen(value)+1 );
			
			// get new val from the user's inner format
			sprintf( formatVal, formatStr, value );
			
			// store final inner string in a tmp var to allow new allocation
			char *tmpStr = estrdup(finalStr);
			
			// resize based on current size plus the new val
			finalStr = (char*)emalloc( strlen(tmpStr)+strlen(formatVal)+1 );
			
			// concatenate the new val
			sprintf( finalStr, "%s%s", tmpStr, formatVal );
			
			// (cleanup) free tmp strings
			EFREE(tmpStr);
			EFREE(formatVal);
			EFREE(value);
		} // end IF
			
	} // end FOR
	
	// return output to user (php takes memory copy)
	RETURN_STRING( finalStr, 0 );
} // end

/** Aug 12, 2006
* - used to create a dynamic repeater that will use assoc arrays, the array count as an implicit multiplier
* and the key & value as the strings to inject in the printf output (walks the array to output final string)
*/
ZEND_FUNCTION( qu_repeat_kv )
{
	zval *userArr = NULL;

	int numArgs = ZEND_NUM_ARGS();
	
	// inner string format
	char *formatStr = (char*)NULL;
	int formatStr_len = 0;
	
	if ( numArgs != 2 )
	{
		OUTPUT( "pass 2 (two) parameters (STRING_FORMAT, ARRAY)." );
	} // end IF

	// get parameters
	if ( zend_parse_parameters(numArgs TSRMLS_CC,
		"sa", &formatStr, &formatStr_len, &userArr )
		== FAILURE )
	{
		OUTPUT( "bad parameter parse." );
	} // end IF
	
	// declare internal assoc array from user array
	HashTable *hash = Z_ARRVAL_P(userArr);
	HashPosition position;
	
 	// key's value
	zval **arrVal = NULL;
	
	// final returned string
	char *finalStr = (char*)NULL;
	finalStr = "";
	
	// loop through all the keys & values
	for( zend_hash_internal_pointer_reset_ex(hash, &position) ; 
	zend_hash_get_current_data_ex(hash, (void**)&arrVal, &position) == SUCCESS ; 
	zend_hash_move_forward_ex(hash, &position) ) 
	{		
		// declare need variables
		char *key = (char*)NULL;
		int key_len = 0;
		long index = 0;
		
		// gets key type
		int result = zend_hash_get_current_key_ex(hash, &key, &key_len, &index, 0, &position);
		
		if ( result == HASH_KEY_IS_LONG )
		{
			OUTPUT( "pass parameter array(STRING=>STRING) only" );
		} // end
		
		// if the key is a string or an index (assoc-array or index-array)
		if ( (result == HASH_KEY_IS_STRING) )
		{
			// assign key's value to char variable
			char *value = estrdup(Z_STRVAL_PP(arrVal));
			
			// free mem for new val
			char *formatVal = (char*)emalloc( formatStr_len+strlen(value)+key_len+1 );
			
			// get new val from the user's inner format
			sprintf( formatVal, formatStr, key, value );
			
			/*if ( !formatVal )
				OUTPUT( "bad format." );*/
				
			// store final inner string in a tmp var to allow new allocation
			char *tmpStr = estrdup(finalStr);
			
			// resize based on current size plus the new val
			finalStr = (char*)emalloc( strlen(tmpStr)+strlen(formatVal)+1 );
			
			// concatenate the new val
			sprintf( finalStr, "%s%s", tmpStr, formatVal );
			
			// (cleanup) free tmp strings
			EFREE(tmpStr);
			EFREE(formatVal);
			EFREE(value);
		} // end IF
			
	} // end FOR
		
	// return output to user (php takes memory copy)
	RETURN_STRING( finalStr, 0 );
} // end

// sends the disabled value to client
ZEND_FUNCTION( send_qudisabled )
{
	IF_CALLBACK
	
	SEND_QURESPONSE(JS_DISABLED);
	
	END_CALLBACK
}

/**
* - used to allow manipulation of the pages timer
* - if the duration is set to 0 (zero) then the timer is cutoff
*/
#define QU_TIMER "qutimer" // ID associated with QuTimer (client side)
ZEND_FUNCTION( qutimer )
{
	int numargs = ZEND_NUM_ARGS();
		
	IF_CALLBACK		
		if ( numargs != 1 )
			OUTPUT( "pass 1 (one) parameter (DURATION)" );
		
		char *duration;
		if ( get_string_parameter(&duration) == FAILURE )
		{
			OUTPUT( "failed parse parameters." );
		} // end IF
		
	 	// output qu function response
		PUTS( js_response(QU_TIMER, JS_TIMER, duration) );
	NOT_CALLBACK
		
		char *func = NULL;
		int func_len;
		
		if ( numargs != 1 )
			OUTPUT( "pass 1 (one) parameter (TICK_FUNCTION)" );

		if ( zend_parse_parameters(numargs TSRMLS_DC,
			"s", &func, &func_len ) == FAILURE )
		{
			OUTPUT( "failed parse parameters." );
		} // end IF
				
		ECHO( "<input type='hidden' qufunc='%s' name='"QU_TIMER"' id='"QU_TIMER"' />", func );
		
	END_CALLBACK
}
