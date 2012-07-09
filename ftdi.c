#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "php.h"
#include "php_ini.h"

#include <ftdi.h>
#include "php_ftdi.h"

/* resource list entry */
static int le_php_ftdi_context;
/* exception class entry */
static zend_class_entry *php_ftdi_exception_sc_entry;

/* resource destructor */
static void php_ftdi_context_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    struct ftdi_context *context = (struct ftdi_context*) rsrc->ptr;

    if (context) {
        ftdi_usb_close(context);
        ftdi_free(context);
    }
}

ZEND_MINIT_FUNCTION(php_ftdi) {
    le_php_ftdi_context = zend_register_list_destructors_ex(
        php_ftdi_context_dtor, NULL, PHP_FTDI_CONTEXT_RES_NAME, module_number
    );

    REGISTER_LONG_CONSTANT("FTDI_BITMODE_BITBANG", BITMODE_BITBANG, CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_CBUS",    BITMODE_CBUS,    CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_MCU",     BITMODE_MCU,     CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_MPSSE",   BITMODE_MPSSE,   CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_OPTO",    BITMODE_OPTO,    CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_RESET",   BITMODE_RESET,   CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_SYNCBB",  BITMODE_SYNCBB,  CONST_CS | CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("FTDI_BITMODE_SYNCFF",  BITMODE_SYNCFF,  CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}

ZEND_NAMED_FUNCTION(php_ftdi_init)
{
    struct ftdi_context *context = ftdi_new();
    int resource_id;

    ftdi_init(context);

    resource_id = ZEND_REGISTER_RESOURCE(return_value, context, le_php_ftdi_context);

    unsigned char data = 64;

    RETURN_RESOURCE(resource_id);
}

ZEND_NAMED_FUNCTION(php_ftdi_usb_open)
{
    struct ftdi_context *context;
    long vendor_id = 0;
    long product_id = 0;
    zval *zcontext;
    int ret = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zcontext, &vendor_id, &product_id) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(context, struct ftdi_context*, &zcontext, -1, PHP_FTDI_CONTEXT_RES_NAME, le_php_ftdi_context);

    ret = ftdi_usb_open(context, vendor_id, product_id);

    if (ret != 0) {
        PHP_FTDI_THROW(context, ret);
    }

    RETURN_LONG((long) ret);
}

ZEND_NAMED_FUNCTION(php_ftdi_usb_close)
{
    struct ftdi_context *context;
    zval *zcontext;
    int ret = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zcontext) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(context, struct ftdi_context*, &zcontext, -1, PHP_FTDI_CONTEXT_RES_NAME, le_php_ftdi_context);

    ret = ftdi_usb_close(context);

    if (ret != 0) {
        PHP_FTDI_THROW(context, ret);
    }

    RETURN_LONG((long) ret);
}

ZEND_NAMED_FUNCTION(php_ftdi_set_bitmode)
{
    struct ftdi_context *context;
    long bitmask;
    long mode;
    zval *zcontext;
    int ret = -1;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rll", &zcontext, &bitmask, &mode) == FAILURE) {
        return;
    }

    ZEND_FETCH_RESOURCE(context, struct ftdi_context*, &zcontext, -1, PHP_FTDI_CONTEXT_RES_NAME, le_php_ftdi_context);

    ret = ftdi_set_bitmode(context, bitmask, mode);

    if (ret != 0) {
        PHP_FTDI_THROW(context, ret);
    }

    RETURN_LONG((long) ret);
}

ZEND_NAMED_FUNCTION(php_ftdi_write_data)
{
    struct ftdi_context *context;
    long *buf = NULL;
    int ret = -1;
    zval *zcontext;
    unsigned char data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl", &zcontext, &buf) == FAILURE) {
        return;
    }

    data = (unsigned char) (long) buf;

    ZEND_FETCH_RESOURCE(context, struct ftdi_context*, &zcontext, -1, PHP_FTDI_CONTEXT_RES_NAME, le_php_ftdi_context);

    /* -666 device unavailable, <0 error code from usb_bulk_write, >0 number of bytes written */
    ret = ftdi_write_data(context, &data, sizeof(data));
    
    if (ret < 0) {
        PHP_FTDI_THROW(context, ret);
    }

    RETURN_LONG((long) ret);
}

zend_function_entry php_ftdi_functions[] = {
  ZEND_NAMED_FE(ftdi_init, php_ftdi_init, NULL)
  ZEND_NAMED_FE(ftdi_usb_open, php_ftdi_usb_open, NULL)
  ZEND_NAMED_FE(ftdi_usb_close, php_ftdi_usb_close, NULL)
  ZEND_NAMED_FE(ftdi_set_bitmode, php_ftdi_set_bitmode, NULL)
  ZEND_NAMED_FE(ftdi_write_data, php_ftdi_write_data, NULL)
  { NULL, NULL, NULL }
};

zend_module_entry php_ftdi_module_entry = {
  STANDARD_MODULE_HEADER,
  PHP_FTDI_EXTNAME,
  php_ftdi_functions,
  PHP_MINIT(php_ftdi),
  NULL,
  NULL,
  NULL,
  NULL,
  PHP_FTDI_VERSION,
  STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(php_ftdi)