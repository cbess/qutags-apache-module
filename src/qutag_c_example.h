/* C. Bess Creation - QuTags.com - Quantum Quinn - 2006 */
// used as repository for example ZEND API code 

// --> create an associative array
zval *arr;
ALLOC_INIT_ZVAL( arr );
array_init( arr );

add_assoc_string( arr, "qq", "Quantum Quinn", 1 );
add_assoc_string( arr, "cbess", "C. Bess Creation", 1 );
// <-- end

// --> START create an zval (internal) variables
zval *aID;
zval *aLong;
char *exitFunc = estrdup("phpinfo");

ALLOC_INIT_ZVAL( aID );
ALLOC_INIT_ZVAL( aLong );

ZVAL_STRING( aID, exitFunc, strlen(exitFunc) );
	
ZVAL_LONG( aLong, 777 );

zval **params [3];
params[0] = &aID;
params[1] = &aLong;
params[2] = &arr;
// <-- END

// --> START get parameters from the user
zval *userFunc;
zval *userArr;

char final[100];
int numArgs = ZEND_NUM_ARGS();
char *tagId = (char*)NULL;
int tagId_len;
char *tagValue = (char*)NULL;
int tagValue_len;

if ( (numArgs != 4) )
{
	WRONG_PARAM_COUNT;
} // end IF

// --> get parameters
if ( zend_parse_parameters(numArgs TSRMLS_CC,
	"ssza", &tagId, &tagId_len, &tagValue, &tagValue_len, &userFunc, &userArr )
	== FAILURE )
{
	RETURN_STRING( "Check parameters.", 1 );
} // end IF

if ( userFunc->type != IS_STRING )
{
	RETURN_STRING( "this function requires string arguments", 1 );
} // end IF
// <-- end

// --> start
zval **arrVal;
char *val = (char*)NULL;
// get string values from assoc array
if  ( zend_hash_find(Z_ARRVAL_P(userArr) /*assoc array to search*/, 
"qq" /*key str*/, 3/*str length*/, 
(void**)&arrVal) == FAILURE )
{
	RETURN_STRING( "bad assoc array", 1 );
} // end IF
else
{
	val = Z_STRVAL_PP(arrVal);
}	
// <-- end

// --> get int values from assoc array
if  ( zend_hash_index_find(Z_ARRVAL_P(assocArr), // array to search 
keyVal, // index 
(void**)arrVal) == FAILURE )
{
	return FALSE;
} // end IF
else
{		
	return TRUE;
} // end ELSE
// <-- end

// --> array walker
static int arraywalker( zval *assocArr, int keyVal, zval ***arrVal )
{
	HashTable *hash = Z_ARRVAL_P(assocArr);
	HashPosition position;

	for( zend_hash_internal_pointer_reset_ex(hash, &position) ; 
	zend_hash_get_current_data_ex(hash, (void**)arrVal, &position) == SUCCESS ; 
	zend_hash_move_forward_ex(hash, &position) ) 
	{		
		char *key = (char*)NULL;
		int key_len = 0;
		long index = 0;
		int result = zend_hash_get_current_key_ex(hash, &key, &key_len, &index, 0, &position);

		if ( result == HASH_KEY_IS_LONG )
		{
			if ( index == (long)keyVal )
				return TRUE;
		} // end IF
		else if ( result == HASH_KEY_IS_STRING )
		{
		} // end ELSE

		// reset current key params
		index = 0;
		key = NULL;
		arrVal = NULL;
	} // end FOR

	return FALSE;

}
// <-- end array walker

// --> start call function example (with return value)
char *funcName = estrdup("");

zval *retVal = NULL;

TSRMLS_FETCH();

zval *arg = NULL;
char *strArg = estrdup("");
MAKE_STD_ZVAL( arg );
ZVAL_STRING( arg, strArg, strlen(strArg) );

zval **params[1];
params[0] = &arg;

retVal = call_function_ex( funcName, 1, params );

convert_to_string(retVal);
char *answer = estrdup(Z_STRVAL_P(retVal));
php_printf( "returned: %s", answer );
// <-- end

// END
