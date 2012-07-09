php_ftdi
========

Libftdi PHP extension for talking to FTDI devices.

Build instructions
------------------

sudo apt-get install libftdi1 libftdi-dev php5-dev

cd /tmp
git clone https://github.com/kaiwa/php_ftdi.git
cd php_ftdi
phpize
./configure
make
sudo make install
