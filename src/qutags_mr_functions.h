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

static void php_qutags_init_globals( zend_qutags_globals *qutags_globals )
{
}

ZEND_MINIT_FUNCTION( minit_qutags )
{
	/* START CONSTANTS (case insensitive) */
	// MISC
	REGISTER_LONG_CONSTANT( "quPAGE", quPAGE, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "quANY_NAME", quANY_NAME, CONST_PERSISTENT );
	
	// PROPERTIES
	REGISTER_LONG_CONSTANT( "quvalue", quVALUE, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "quinnerhtml", quINNER_HTML, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "qudisplay", quDISPLAY, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "quclass", quCLASS, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "qudisabled", quDISABLED, CONST_PERSISTENT );
	
	// EVENTS
	REGISTER_LONG_CONSTANT( "quclick", quON_CLICK, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "qumouseout", quON_MOUSE_OUT, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "qumouseover", quON_MOUSE_OVER, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "quchange", quON_CHANGE, CONST_PERSISTENT );	
	REGISTER_LONG_CONSTANT( "qublur", quON_BLUR, CONST_PERSISTENT );
	REGISTER_LONG_CONSTANT( "qukeyup", quON_KEY_UP, CONST_PERSISTENT );
	
	//REGISTER_LONG_CONSTANT( "qu", qu, CONST_PERSISTENT );
	//REGISTER_LONG_CONSTANT( "qu", qu, CONST_PERSISTENT );
	//REGISTER_LONG_CONSTANT( "qu", qu, CONST_PERSISTENT );
	/* END CONSTANTS */
	
	// GLOBALS
	ZEND_INIT_MODULE_GLOBALS( qutags, php_qutags_init_globals, NULL );
	
	return SUCCESS;
}

ZEND_MSHUTDOWN_FUNCTION( mshutdown_qutags )
{
	return SUCCESS;
}

ZEND_RINIT_FUNCTION( rinit_qutags )
{
	QUTAGS_G(qu_response) = estrdup("");
	
	// set initial value of the current request (not a callback)
	QUTAGS_G(is_callback) = 0;
			
	return SUCCESS;
}

ZEND_RSHUTDOWN_FUNCTION( rshutdown_qutags )
{	
	return SUCCESS;
} // end IF

// displays info in phpinfo() function
ZEND_MINFO_FUNCTION( minfo_qutags )
{
	php_info_print_table_start();
	php_info_print_table_header(2, "Key", "Value");
	php_info_print_table_row(2, "Version", "ALPHA");
	php_info_print_table_row(2, "License", "DEV / UNRELEASED");
	php_info_print_table_end();
}
