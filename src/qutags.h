/* C. Bess Creation (Quantum Quinn) */
// QuTags - Thu Jul 13 2006
// updated: 2006 Jul 18
// Copyright (C) 2006, 2007 Christopher Bess of Quantum Quinn
// - C. Bess Creation - <qutags@quantumquinn.com>
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

#ifndef _QUTAGS_H_
#define _QUTAGS_H_ 1

extern zend_module_entry qutags_module_entry;
#define phpext_qutags_ptr &qutags_module_entry

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_BEGIN_MODULE_GLOBALS(qutags)
	char *qu_response;
	int is_callback;
ZEND_END_MODULE_GLOBALS(qutags)

#ifdef ZTS
#define QUTAGS_G(v) TSRMG(qutags_globals_id, zend_qutags_globals*, v )
#else
#define QUTAGS_G(v) (qutags_globals.v)
#endif

#define DEBUG 1

// php_printf alias
#define ECHO php_printf
// end

/* CONSTANTS */
enum {
// misc
/*
	- quANY_NAME is for unconventional use only
	- essentially just causes the request to be sent to the server
*/
	quANY_NAME = -1, // unconventional use only
	quPAGE = 1,
	
// properties
 	quVALUE,
	quINNER_HTML,
	quDISPLAY,
	quCLASS,
	quDISABLED,
	
// events
 	quON_CLICK,
	quON_MOUSE_OUT,
	quON_MOUSE_OVER,
	quON_CHANGE,
	quON_BLUR, // 9
	quON_KEY_UP
};
/* END */

/*
	function will be named lower case as:
	qutag_button, qutag_div, etc
*/
ZEND_MINIT_FUNCTION( minit_qutags );
ZEND_MSHUTDOWN_FUNCTION( mshutdown_qutags );
ZEND_RINIT_FUNCTION( rinit_qutags );
ZEND_RSHUTDOWN_FUNCTION( rshutdown_qutags );
ZEND_MINFO_FUNCTION( minfo_qutags );

// init qutag engine
ZEND_FUNCTION( init_qutags );

// html output functions
ZEND_FUNCTION( quform_start );
ZEND_FUNCTION( quform_end );
ZEND_FUNCTION( qulistbox );
ZEND_FUNCTION( qulistbox_start );
ZEND_FUNCTION( qulistbox_end );
ZEND_FUNCTION( quoption );
ZEND_FUNCTION( qutextbox );
ZEND_FUNCTION( qutextarea );
ZEND_FUNCTION( qutextarea_start );
ZEND_FUNCTION( qutextarea_end );
ZEND_FUNCTION( qubutton );
ZEND_FUNCTION( qudiv );
ZEND_FUNCTION( qudiv_start );
ZEND_FUNCTION( qudiv_end );
ZEND_FUNCTION( quhyperlink );
ZEND_FUNCTION( quradiobutton );
ZEND_FUNCTION( qucheckbox );
ZEND_FUNCTION( qufloatingdiv_start );
ZEND_FUNCTION( qufloatingdiv_end );
ZEND_FUNCTION( quimage );

// misc funcs
ZEND_FUNCTION( qustatus );
ZEND_FUNCTION( qu_add_option );
ZEND_FUNCTION( qu_clear_options );
ZEND_FUNCTION( qutimer );

// output helpers
ZEND_FUNCTION( qu_echo );
ZEND_FUNCTION( qu_return );
ZEND_FUNCTION( qu_repeat );
ZEND_FUNCTION( qu_repeat_kv );

// general callback func
ZEND_FUNCTION( quresponse );

// callback specific funcs
ZEND_FUNCTION( send_quvalue );
ZEND_FUNCTION( send_quinnerhtml );
ZEND_FUNCTION( send_qudisplay );
ZEND_FUNCTION( send_qudisabled );
ZEND_FUNCTION( send_jscode );

#endif /* _QUTAGS_H_ */
