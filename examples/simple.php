<?php

try {
    $ftdi = ftdi_init();

    ftdi_usb_open($ftdi, 0x0403, 0x6001);
    ftdi_set_bitmode($ftdi, 0xff, FTDI_BITMODE_BITBANG);
    ftdi_write_data($ftdi, 0);

} catch (Exception $e) {
    var_dump($e);
}

