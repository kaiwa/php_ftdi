#ifndef PHP_FTDI_H
    #define PHP_FTDI_H 1

    #define PHP_FTDI_EXTNAME "ftdi"
    #define PHP_FTDI_VERSION "0.01"

    #define PHP_FTDI_CONTEXT_RES_NAME "FTDIContext"

    extern zend_module_entry php_ftdi_module_entry;
    #define phpext_ftdi_ptr &php_ftdi_module_entry


    #define PHP_FTDI_THROW(context, code) \
        zend_throw_exception(php_ftdi_exception_sc_entry, ftdi_get_error_string(context), (long)code TSRMLS_CC); \
        return;
#endif
