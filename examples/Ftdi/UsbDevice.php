<?php

namespace Ftdi;

class UsbDevice
{
    const BITMODE_BITBANG = FTDI_BITMODE_BITBANG;
    const BITMODE_CBUS    = FTDI_BITMODE_CBUS;
    const BITMODE_MCU     = FTDI_BITMODE_MCU;
    const BITMODE_OPTO    = FTDI_BITMODE_OPTO;
    const BITMODE_RESET   = FTDI_BITMODE_RESET;
    const BITMODE_SYNCBB  = FTDI_BITMODE_SYNCBB;
    const BITMODE_SYNCFF  = FTDI_BITMODE_SYNCFF;

    protected $vendorId, $productId;

    public function __construct($vendorId, $productId)
    {
        $this->vendorId  = $vendorId;
        $this->productId = $productId;
        $this->handle    = ftdi_init();

        ftdi_usb_open($this->handle, $vendorId, $productId);
    }

    public function setBitMode($mask, $mode)
    {
        ftdi_set_bitmode($this->handle, $mask, $mode);
    }

    public function write($data)
    {
        ftdi_write_data($this->handle, $data);
    }

    public function __destruct()
    {
        ftdi_usb_close($this->handle);
    }
 }