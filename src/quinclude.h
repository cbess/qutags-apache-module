/*
	- Provides the needed helper utility components
	created: Jul 18, 2006
	updated: 2006 Aug 15

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

#ifndef _QUINCLUDE_H_
#define _QUINCLUDE_H_

#define FALSE 0
#define TRUE 1

#define NL_REPLACE "~!" // replaced on client side with new line
#define SLASH_REPLACE "~%"
#define USER_NL "!NL!" // replaced by client js with NL_REPLACE (if user typed the NL_REPLACE)
#define NEW_LINE "\\n"
#define USER_SLASH "!S!"

// used to shortcut the js output for the quarray sent to the client
#define JS_INNER_HTML "IH" // element.innerHTML
#define JS_VALUE "V" // element.value
#define JS_DISPLAY "DIS" // style.display
#define JS_CLASS "CSS" // className
#define JS_OPTION "OPT" // new Option(...)
#define JS_IMG_SRC "SRC" // image.src
#define JS_DISABLED "DS" // element.enabled
#define JS_TIMER "TMR" // the qutimer
#define JS_CODE "JSC" // js eval code

#define JS_RESPONSE_TEMPLATE "'%s@%s':'%s'," // the template for all js response (server to client)
 //#include "php.h" // done by outer files

// declarations
static zval * call_function_full( zval *funcName, int param_count, zval **params[] );
static int get_assoc_array_svalue( zval *assocArr, char *key, int key_length, zval ***arrVal );
static int get_assoc_array_ivalue( zval *assocArr, int key, zval ***arrVal );
static char *append_to_quresponse( char * string );
static char *qurpc_event( char *id, char *func, int type );
// end

/* DEFINES MACROS */

// zval destuctor alias
#define DTOR(x) \
if ( x ) { zval_ptr_dtor(x); x = NULL; }
// end

// efree alias
#define EFREE(x)	\
if ( x ) { efree(x); x = NULL; }
// end

#ifdef DEBUG
#define OUTPUT(x)	\
RETURN_STRING(x, 1)
#else
#define OUTPUT(x)	\
php_error(E_WARNING, "%s "##x, get_active_function_name(TSRMLS_C))
#endif

// declars params used with all qutag args
#define QUTAG_PARAMS	\
	char *tagId = (char*)NULL;	\
	zval *tagArr = NULL;	\
	zval **arrVal = NULL;

// initializes args from parameters
// fetches the qutag arguments to get the parameters to use internally
#define INIT_QUTAG_ARGS		   									 \
	QUTAG_PARAMS			   									 \
	if ( ZEND_NUM_ARGS() == 0 )									 \
	{						   									 \
		OUTPUT( "pass 2 (two) parameters (NAME [, ASSOC_ARRAY])" ); \
	}															 \
																 \
	if ( get_qutag_parameters( &tagId, &tagArr, ZEND_NUM_ARGS() ) == FAILURE )	 \
	{															 \
		OUTPUT( "error: get_qutag_parameters" );				 \
	}
// end	

// --> start GET QUTAG ASSOC ARRAY VALUES

// get (int indexed) value
// use: AARRAY_IVALUE( (char*)return_value, quCONST );
#define AARRAY_IVALUE(x, y)	\
x = (char*)NULL;			\
if (get_assoc_array_ivalue( tagArr, y, &arrVal ))	\
{	x = estrdup(Z_STRVAL_PP(arrVal));	}	\
else {	x = estrdup("");	}
// end

// get (string indexed) value
// use: AARRAY_SVALE( "the_key", (char*)return_value );
#define AARRAY_SVALUE(y, x)	\
x = (char*)NULL;			\
if (get_assoc_array_svalue( tagArr, y, strlen(y), &arrVal ))	\
{	x = estrdup(Z_STRVAL_PP(arrVal));	}			\
else {	x = estrdup("");	}

// properties
#define QUTAG_quVALUE(x)	\
AARRAY_IVALUE(x, quVALUE)

#define QUTAG_quINNER_HTML(x)	\
AARRAY_IVALUE(x, quINNER_HTML)

#define QUTAG_quDISPLAY(x)	\
AARRAY_IVALUE(x, quDISPLAY)

#define QUTAG_quCLASS(x)	\
AARRAY_IVALUE(x, quCLASS)

#define QUTAG_quDISABLED(x)	\
AARRAY_IVALUE(x, quDISABLED)

// events
#define QUTAG_quON_CLICK(x)	\
AARRAY_IVALUE(x, quON_CLICK)

#define QUTAG_quON_MOUSE_OVER(x)	\
AARRAY_IVALUE(x, quON_MOUSE_OVER)

#define QUTAG_quON_MOUSE_OUT(x)	\
AARRAY_IVALUE(x, quON_MOUSE_OUT)

#define QUTAG_quON_CHANGE(x)	\
AARRAY_IVALUE(x, quON_CHANGE)

#define QUTAG_quON_BLUR(x)	\
AARRAY_IVALUE(x, quON_BLUR)

#define QUTAG_quON_KEY_UP(x)	\
AARRAY_IVALUE(x, quON_KEY_UP)
// <-- end GET ASSOC VALUE

// is callback condition
#define IF_CALLBACK if ( iscallback() ) {
#define NOT_CALLBACK } else {
#define IF_NOT_CALLBACK if( !iscallback() ) {
#define END_CALLBACK }
// end

// --> qurpc events
#define EVT(x,y)	\
qurpc_event(tagId, x, y)

#define EVT_ON_CLICK(x) 	\
EVT(x, quON_CLICK)
 
#define EVT_ON_MOUSE_OVER(x) 	\
EVT(x, quON_MOUSE_OVER)

#define EVT_ON_MOUSE_OUT(x) 	\
EVT(x, quON_MOUSE_OUT)

#define EVT_ON_CHANGE(x) 	\
EVT(x, quON_CHANGE)

#define EVT_ON_BLUR(x)	\
EVT(x, quON_BLUR)

#define EVT_ON_KEY_UP(x)	\
EVT(x, quON_KEY_UP)
// <-- end

// alias
#define QUTAG_FUNCTION(x) 	\
ZEND_FUNCTION(x)
// end

// --> alias append_quresponse
// sends response to client js
#define SEND(x,y)		\
x = (char*)NULL;			\
if (get_assoc_array_ivalue( tagArr, y, &arrVal ))	\
{	x = estrdup(Z_STRVAL_PP(arrVal));	append_quresponse(tagId, x, y); arrVal = NULL;	}
	
#define SEND_quVALUE(x)		\
SEND(x, quVALUE)

#define SEND_quDISPLAY(x)		\
SEND(x, quDISPLAY)

#define SEND_quINNER_HTML(x)		\
SEND(x, quINNER_HTML)

#define SEND_quCLASS(x) 	\
SEND(x, quCLASS)

#define SEND_quDISABLED(x)	\
SEND(x, quDISABLED)

// sends single property specific response without the array argument
#define SEND_QURESPONSE(x)	\
	char *tagId = NULL; 	\
	char *value = NULL; 	\
	int tagId_len; 			\
	int value_len;			\
	if ( ZEND_NUM_ARGS() != 2 ) 	\
		OUTPUT( "pass 2 (two) parameters (NAME, STRING)" ); 	\
																\
	if ( zend_parse_parameters(2 TSRMLS_DC,						\
		"ss", &tagId, &tagId_len, &value, &value_len ) == FAILURE ) 	\
	{																	\
		OUTPUT( "failed parse parameters." );							\
	}																	\
	ECHO( JS_RESPONSE_TEMPLATE, x, tagId, escape_string(value) );
// <-- end

// --> start single tag
#define ECHO_END_TAG(x) 	\
char *end = estrdup(x);		\
PHPWRITE( end, strlen(end) );	\
EFREE(end);
// <--

// --> start declares and set common qutag values
#define QUTAG_quVALUE_DISPLAY_CLASS_DISABLED	\
char *tagVal;	\
char *display;	\
char *css;		\
char *disabled;	\
QUTAG_quVALUE(tagVal);	\
QUTAG_quDISPLAY(display);	\
QUTAG_quCLASS(css);			\
QUTAG_quDISABLED(disabled);
// <-- end

// --> start
#define IS_DISABLED 	\
(strlen(disabled) ? " DISABLED " : "")
// <--

/* END MACROS*/

// START HELPER FUNCTIONS (shortcut)
// converts a string (char*) to an zval
static zval * string_to_zval( char *str )
{	
	zval *string;

	ALLOC_INIT_ZVAL( string );
	ZVAL_STRING( string, str, strlen(str) );

	return string;
}

// converts the input string to lower case
void strlwr( char *s )
{   		
   if ( s )
   {    
		for ( ; *s; ++s )
	    	*s = tolower(*s);
   } // end IF
}
// END HELPER FUNCTIONS

/* START CALL PHP ENV FUNCTIONS

	int call_user_function_ex ( 
	HashTable* function_table, zval** object_pp, 
	zval* function_name, zval** retval_ptr_ptr, 
	zend_uint param_count, zval** params[], 
	int no_separation, HashTable* symbol_table TSRMLS_DC )
	
	it can call php only functions. ex: phpinfo, etc.
*/
// call php env function without a return value
static zval * call_function_ex( char *funcName, int param_count, zval ***params )
{
	zval *func;
	
	MAKE_STD_ZVAL( func );
	ZVAL_STRING( func, funcName, strlen(funcName) );
		
	// function call
	return call_function_full( func, param_count, params );
}

static void call_function( char *funcName )
{
	call_function_ex( funcName, 0, (zval***)NULL );
	
	return;
}

static zval * call_function_full( zval *funcName, int param_count, zval **params[] )
{	
	zval *retVal;
	
	TSRMLS_FETCH();
	
	if ( call_user_function_ex(
	CG(function_table), NULL, funcName, &retVal, param_count, params, 0, NULL TSRMLS_CC) 
	!= SUCCESS )
	{
		return (zval*)NULL;
	} // end IF
		
	DTOR(funcName);
	
	return retVal;
}

// calls a function with one string arg
static char * call_function_one( char *func, char *strArg )
{
	zval *retVal = NULL;

	zval *arg = NULL;
	MAKE_STD_ZVAL( arg );
	ZVAL_STRING( arg, strArg, strlen(strArg)+1 );

	zval **params[1];
	params[0] = &arg;

	retVal = call_function_ex( func, 1, params );

	char *answer = estrdup(Z_STRVAL_P(retVal));
	
	return answer;
}

/*  END CALL PHP ENV FUNCTIONS */

/* START CLEAN UP TEXT FUNCTIONS */
// char *php_addslashes(char *str, int length, int *new_length, int should_free TSRMLS_DC)
// escapes the quotation in a string (single and double)
static char * escape_string( char *str )
{
	int len;
	return (char*)php_addslashes(str, strlen(str), &len, 0);
}

/* START PARSE PARAMETERS */

static int get_qutag_parameters( char **id, zval **assocArr, int numArgs )
{
	int id_len;
	
	switch ( numArgs )
	{
		// if only supplying the id/name
		case 1:
		assocArr = NULL;
		return zend_parse_parameters(1 TSRMLS_DC,
			"s", id, &id_len );
			
		// if utilizing qutag properties/options
		case 2:
		return zend_parse_parameters(2 TSRMLS_DC,
			"sz", id, &id_len, assocArr );
			
		default:
		return FAILURE;
	} // end SWITCH
}

static int get_string_parameter( char **value )
{
	int value_len;
	
	return zend_parse_parameters( 1 TSRMLS_DC,
	"s", value, &value_len );
}
/* END START PARSE PARAMETERS */

/* GET HASH ENTRY (ASSOC_ARRAY VALUE) */
// key as string
static int get_assoc_array_svalue( zval *assocArr, char *keyVal, int key_length, zval ***arrVal )
{
		if ( !assocArr ) 
			return FALSE;
	
		HashTable *hash = Z_ARRVAL_P(assocArr);
		HashPosition position;
	
		for( zend_hash_internal_pointer_reset_ex(hash, &position) ; 
		zend_hash_get_current_data_ex(hash, (void**)arrVal, &position) == SUCCESS ; 
		zend_hash_move_forward_ex(hash, &position) ) 
		{		
			char *key = (char*)NULL;
			int key_len = 0;
			long index = 0;
	
			// get current key
			int result = zend_hash_get_current_key_ex(hash, &key, &key_len, &index, 0, &position);
	
			if ( result == HASH_KEY_IS_STRING )
			{
				// if match found
				if ( !strcasecmp( keyVal, key) )
				{
//					convert_to_string(*arrVal);
					return TRUE;
				} // end IF
			} // end IF
	
		} // end FOR
	
	return FALSE;
}

// key as int (long) or string
static int get_assoc_array_ivalue( zval *assocArr, int keyVal, zval ***arrVal )
{
	if ( !assocArr ) 
		return FALSE;
	
	HashTable *hash = Z_ARRVAL_P(assocArr);
	HashPosition position;

	for( zend_hash_internal_pointer_reset_ex(hash, &position) ; 
	zend_hash_get_current_data_ex(hash, (void**)arrVal, &position) == SUCCESS ; 
	zend_hash_move_forward_ex(hash, &position) ) 
	{		
		char *key = (char*)NULL;
		int key_len = 0;
		long index = 0;
		
		// get current key
		int result = zend_hash_get_current_key_ex(hash, &key, &key_len, &index, 0, &position);
	
		if ( result == HASH_KEY_IS_LONG )
		{
			// if match found
			if ( index == (long)keyVal )
				return TRUE;
		} // end IF
		
	} // end FOR
	
	return FALSE;
}
/* END GET HASH ENTRY */

/* GLOBALS VAR FUNCTIONS */
static char * get_quresponse()
{
	return QUTAGS_G(qu_response);
}

static char *append_to_quresponse( char * string )
{
	// get full length
	int size = strlen( get_quresponse() );
	size += strlen(string);
	
	// free mem
	char *oldStr = (char*)emalloc(size+1);
	
	// combine strings
	sprintf( oldStr, "%s%s", get_quresponse(), string );

	// free mem and assign value
	QUTAGS_G(qu_response) = estrdup(oldStr);
	
	// assign value
//	QUTAGS_G(qu_response) = oldStr;
	
	// free mem
	efree(oldStr);
	
	// return the new
	return get_quresponse();
}

static int iscallback()
{
	return QUTAGS_G(is_callback);
}

static void set_iscallback( int iscallback )
{
	QUTAGS_G(is_callback) = iscallback;
}
/* END GLOBAL VARS FUNCTIONS */


// used only during ajax callbacks
static char *js_response( char *id, char *property, char *val )
{
	// get the size of the request
	int size = strlen(id)+strlen(property)+strlen(val)+1;
	
	// get js response store size (from template)
	char *tmp = estrdup( "'@':''," ); // check JS_RESPONSE_TEMPLATE
	
	// get updated size
	size = strlen(tmp)+size;
	
	// cleanup tmp
	efree(tmp);
	
	// free mem (needed [must be accurate])
	char *final = (char*)emalloc(size);
	
	// assign value to final
	// property @ ID notation (read: the property at ID)
	sprintf( final, JS_RESPONSE_TEMPLATE, property, id, val );
	
	return final;
}

// sends the js response to the client via the output buffer (php_body_write)
static void append_quresponse( char *id, char *val, int type )
{
	char *str;
	int isEmpty = (!strlen(val));
	
	// prevents sending empty values based on type
	if ( isEmpty ) 
	{
		switch ( type )
		{
			case quCLASS:
			case quDISPLAY:
			case quDISABLED:
			return;
		} // end IF
	} // end IF
	
	switch( type )
	{
		case quVALUE:
		str = estrdup(JS_VALUE);
		
		if ( !isEmpty)
			val = escape_string(val);
		break;
	
		case quINNER_HTML:
		str = estrdup(JS_INNER_HTML);
		
		if ( !isEmpty )
			val = escape_string(val);
		break;
	
		case quDISPLAY:
		str = estrdup(JS_DISPLAY);
		break;
	
		case quCLASS:
		str = estrdup(JS_CLASS);
		break;
		
		case quDISABLED:
		str = estrdup(JS_DISABLED);
		break;
	} // end SWITCH

	// output response for ajax callback
	PUTS( js_response( id, str, val ) );
	
	// free mem
	efree(str);
	efree(val);
}

// declared here to prevent use elsewhere (and so I can see it)
#define _RPC_ "=\"qurpc('%s','%s')\" "
#define JS_EVT(x)	\
evt = estrdup(" "x _RPC_)

// outputs text for qurpc events
// the function below should ONLY be called from macros
static char *qurpc_event( char *id, char *func, int type )
{	
	if ( !func ) // not null
		return "";
	else if ( !strlen(func) ) // detects if string is empty
			return "";

	char *rpc = (char*)NULL;
	char *evt = (char*)NULL;
	
	// get size of id and func + null char
	int size = strlen(id)+strlen(func)+1;
	
	switch( type )
	{
		case quON_CLICK:
		JS_EVT("onclick");
		break;
		
		case quON_MOUSE_OVER:
		JS_EVT("onmouseover");
		break;
		
		case quON_MOUSE_OUT:
		JS_EVT("onmouseout");
		break;
		
		case quON_CHANGE:
		JS_EVT("onchange");
		break;
		
		case quON_BLUR:
		JS_EVT("onblur");
		break;
		
		case quON_KEY_UP:
		JS_EVT("onkeyup");
		break;
	} // end SWITCH

	// add extra size
	size += strlen(evt);
	
	// alloc mem
	rpc = (char*)emalloc(size);
	
	// create js event signature
	sprintf( rpc, evt, id, func );
	
	// free mem
	EFREE(evt);
	
	return rpc;
}

#endif /* _QUINCLUDE_H_ */
