This is a simple app to write some values to the EC of certain MSI laptops.

It was tested on a GE78 Raider.

This mimics what MSI Center does in Windows to set default fan temperature thresholds, with
higher speeds at the top. It also puts the EC in "performance" mode.

To run:

modprobe ec_sys
echo 1 > /sys/module/ec_sys/parameters/write_support
