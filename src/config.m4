
PHP_ARG_ENABLE( qutags, if enabling the QuTags module, [ --enable-qutags enables qutags ] )

if test "$PHP_QUTAGS" != "no"; then
	AC_DEFINE(COMPILE_DL_QUTAGS, 1, [if compiling QuTag module])
	PHP_NEW_EXTENSION( qutags, qutags.c, $ext_shared )
fi