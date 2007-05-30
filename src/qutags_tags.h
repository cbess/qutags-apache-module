/* 
	created: july 19, 2006
	- used for all qutag tags
	updated: 2006 July 30
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

/*
	tagId = is the identifier for the element (but it actually is the value of the name attribute)
	- so any ID is really the name associated with the element
*/

// qutags
QUTAG_FUNCTION( qu_form_start )
{
		 
	if ( ZEND_NUM_ARGS() != 0 )
	{
		char *action = NULL;
		if ( get_string_parameter(&action) == FAILURE )
		{
			OUTPUT( "pass 1 (one) parameter (STRING)" );
		}
		else
			ECHO( "<form action='%s' name='quform' id='quform' method='post'>", action );	
	} // end IF
	else
	{
		PUTS( "<form action='#' name='quform' id='quform' method='post'>" );
	} // end ELSE
}

QUTAG_FUNCTION( qu_form_end )
{	
	ECHO_END_TAG( "</form>" );
}

// it is dropdown if the user selects size of 1
QUTAG_FUNCTION( qu_listbox )
{
	INIT_QUTAG_ARGS
	
	char *css;
	QUTAG_quCLASS(css);
	
	char *disabled;
	QUTAG_quDISABLED(disabled);
	
	IF_CALLBACK
		SEND_quCLASS(css);
		SEND_quDISABLED(disabled);
	NOT_CALLBACK
		char *onchange;
		QUTAG_quON_CHANGE(onchange);
		
		ECHO( "<select name='%s' class='%s'%s%s></select>", tagId, css,
		EVT_ON_CHANGE(onchange),
		IS_DISABLED );
	END_CALLBACK
}

QUTAG_FUNCTION( qu_listbox_start )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
	
	char *css;
	QUTAG_quCLASS(css);
	
	char *disabled;
	QUTAG_quDISABLED(disabled);
	
	// get the size of the list box (would mean multi or single select)
	char *size;
	AARRAY_SVALUE("size", size);
	
		char *onchange;
		QUTAG_quON_CHANGE(onchange);
		
		ECHO( "<select name='%s' class='%s'%s%s>", tagId, css,
		EVT_ON_CHANGE(onchange),
		IS_DISABLED );
	END_CALLBACK
}

QUTAG_FUNCTION( qu_option )
{
	IF_NOT_CALLBACK
	
	char *text = NULL;
	char *value = NULL;
	int text_len;
	int value_len;
	int numargs = ZEND_NUM_ARGS();
	
	// 1.) text / 2.)  value
	if ( numargs == 0 )
		OUTPUT( "pass 1 (one) parameter (TEXT [, VALUE])" );

	if ( zend_parse_parameters(numargs TSRMLS_DC,
		"s|s", &text, &text_len, &value, &value_len ) == FAILURE )
	{
		OUTPUT( "failed parse parameters." );
	} // end IF
			
	ECHO( "<option value='%s'>%s</option>", (numargs == 2 ? value : ""), text );
	
	END_CALLBACK
}

QUTAG_FUNCTION( qu_listbox_end )
{
	ECHO_END_TAG( "</select>" );
}

QUTAG_FUNCTION( qu_button )
{
	// declare var and get parameters
	INIT_QUTAG_ARGS
		
	QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
	
	IF_CALLBACK
	{		
		SEND_quDISPLAY(display);
		
		SEND_quVALUE(tagVal);
		
		SEND_quDISABLED(disabled);
	}
	NOT_CALLBACK
		char *status = NULL;
		AARRAY_SVALUE("qustatus", status);
		
		char *theFunc = NULL;
		
		// get on click function
		QUTAG_quON_CLICK(theFunc);
		
		ECHO( 
		"<input type='button' class='%s' name='%s' qustatus='%s' value='%s'%s%s/>",
		 css, tagId, 
		 status,
		 tagVal, EVT_ON_CLICK(theFunc),
		IS_DISABLED );
	END_CALLBACK

}

QUTAG_FUNCTION( qu_textbox )
{
	// declar and get params
	INIT_QUTAG_ARGS
	
	QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
	
	IF_CALLBACK
	  	SEND_quVALUE(tagVal);
		SEND_quDISPLAY(display);
		SEND_quDISABLED(disabled);
	NOT_CALLBACK
		
		// get type encase the user wishes it to behave as a password field
		char *type;
		AARRAY_SVALUE("type", type);
		
		char *keyup;
		QUTAG_quON_KEY_UP(keyup); // get the onkeyup event function
		
		// get size of the text element
		char *size;
		AARRAY_SVALUE("size", size);
		
		// get the max length
		char *max;
		AARRAY_SVALUE("maxlength", max);
		
		// get the on blur event handler
		char *onblur;
		QUTAG_quON_BLUR(onblur);
		
		ECHO( "<input type='%s' size='%s' maxlength='%s' class='%s' name='%s' value='%s' %s%s%s/>",
		(strlen(type) ? type : "text"),  // if no type given then assume text
		size, max,
		css, tagId, tagVal,
		EVT_ON_KEY_UP(keyup),
		EVT_ON_BLUR(onblur),
		IS_DISABLED  );	
	END_CALLBACK
		
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_textarea )
{
	INIT_QUTAG_ARGS
	
	QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
	
	IF_CALLBACK
		
	NOT_CALLBACK
	
		char *rows;
		AARRAY_SVALUE( "rows", rows );
	
		char *cols;
		AARRAY_SVALUE("cols", cols);
		
		ECHO( "<textarea class='%s' name='%s' rows='%s' cols='%s'%s>%s</textarea>",
		css, tagId,
		rows, cols,
		IS_DISABLED,
		tagVal );
		
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_textarea_start )
{
	INIT_QUTAG_ARGS
	
	QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
	
	IF_CALLBACK
	NOT_CALLBACK
		char *rows;
		AARRAY_SVALUE( "rows", rows );
	
		char *cols;
		AARRAY_SVALUE("cols", cols);
	
		ECHO( "<textarea class='%s' name='%s' rows='%s' cols='%s'%s>%s",
		css, tagId, 
		rows, cols,
		IS_DISABLED,
		tagVal );
		
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_textarea_end )
{
	ECHO_END_TAG( "</textarea>" );
}

QUTAG_FUNCTION( qu_div )
{
	// declare and get params
	INIT_QUTAG_ARGS
	
	char *val;
	QUTAG_quINNER_HTML(val);

	char *css;
	QUTAG_quCLASS(css);
		
	IF_CALLBACK
		SEND_quINNER_HTML(val);
		
		SEND_quCLASS(css);
	NOT_CALLBACK
		
		ECHO( "<div name='%s' id='%s' class='%s'>%s</div>", tagId, tagId, css, val );
	
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_div_start )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
		char *css;
		QUTAG_quCLASS(css);
	
		ECHO( "<div name='%s' id='%s' class='%s'>", tagId, tagId, css );
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_div_end )
{
	ECHO_END_TAG( "</div>" );
}

QUTAG_FUNCTION( qu_hyperlink )
{
	INIT_QUTAG_ARGS
	
	char *innerhtml;
	QUTAG_quINNER_HTML(innerhtml);
	
	IF_CALLBACK
		SEND_quINNER_HTML(innerhtml);
	NOT_CALLBACK
		QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
		
		char *status = NULL;
		AARRAY_SVALUE("qustatus", status);
		
		char *clickFunc = NULL;
		QUTAG_quON_CLICK(clickFunc);
		
		char *mOutFunc = NULL;
		QUTAG_quON_MOUSE_OUT(mOutFunc);
		
		char *mOverFunc = NULL;
		QUTAG_quON_MOUSE_OVER(mOverFunc);
		
		
		char *title;
		AARRAY_SVALUE("title", title);
		
		char *target;
		AARRAY_SVALUE("target", target);
		
		ECHO( "<a href='javascript:void(0);' name='%s' qustatus='%s' class='%s' title='%s' target='%s' %s%s%s%s>%s</a>",
		tagId, status, css,
		title,
		(strlen(target) ? target : "self"),
		EVT_ON_CLICK(clickFunc), EVT_ON_MOUSE_OVER(mOverFunc), EVT_ON_MOUSE_OUT(mOutFunc),
		innerhtml,
		IS_DISABLED  );
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_radiobutton )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
		QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
		
		char *clickFunc = NULL;
		QUTAG_quON_CLICK(clickFunc);
		
		char *checked;
		AARRAY_SVALUE("checked", checked);
		
		ECHO( "<input type='radio' name='%s' value='%s' %s%s%s/>",
		tagId,
		tagVal,
		EVT_ON_CLICK(clickFunc),
		(strlen(checked) ? " CHECKED " : ""),
		IS_DISABLED );
		
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_checkbox )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
		QUTAG_quVALUE_DISPLAY_CLASS_DISABLED
		
		char *clickFunc = NULL;
		QUTAG_quON_CLICK(clickFunc);
		
		char *checked;
		AARRAY_SVALUE("checked", checked);
		
		ECHO( "<input type='checkbox' name='%s' value='%s' %s%s%s/>",
		tagId,
		tagVal,
		EVT_ON_CLICK(clickFunc),
		(strlen(checked) ? " CHECKED " : ""),
		IS_DISABLED );
		
	END_CALLBACK
	
	DTOR(arrVal);
}

QUTAG_FUNCTION( qu_floating_div_start )
{
	INIT_QUTAG_ARGS
	
	IF_NOT_CALLBACK
		char *css;
		QUTAG_quCLASS(css);
	
		char *caption;
		AARRAY_SVALUE("caption", caption);
		
		char *position;
		AARRAY_SVALUE("position", position);
			
		ECHO(
		"<div id='%s_qufloatingdiv' name='%s_qufloatingdiv' class='%s'" // <-- title start
		"style='position:%s;text-align:center;cursor:move;'>"
		"<div name='%s_caption' name='%s_caption' onmousedown='qufloatingdiv_mDown(\"%s\");' onmouseup='qufloatingdiv_mUp();'>"
		"%s"
		"</div>" // <-- title end
		"<div id='%s_body' name='%s_body' style='cursor:default;'>", // <-- body start
		tagId, tagId,
		css,
		(strlen(position) ? "absolute" : position),
		tagId,
		tagId,
		tagId,
		caption,
		tagId, tagId
		);
	END_CALLBACK
	
	DTOR(arrVal);
} // end

QUTAG_FUNCTION( qu_floating_div_end )
{
	ECHO_END_TAG( "</div></div>" );
} // end

QUTAG_FUNCTION( qu_image )
{
	INIT_QUTAG_ARGS
	
	char *img;
	AARRAY_SVALUE("src", img);
	
	char *css;
	QUTAG_quCLASS(css);
		
	IF_CALLBACK
	// sends img src	
	if ( strlen(img) )
		PUTS( js_response(tagId, JS_IMG_SRC, img) );
		
		// send css class name
	if ( strlen(css) )
		PUTS( js_response(tagId, JS_CLASS, css) );
	NOT_CALLBACK			
		char *onclick;
		QUTAG_quON_CLICK(onclick);
		
		ECHO( "<img id='%s' name='%s' src='%s' class='%s' %s/>",
		tagId, tagId,
		img, css,
		EVT_ON_CLICK(onclick) );
	END_CALLBACK
} // end
