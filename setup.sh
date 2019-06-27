# give printk enough permission
echo "7 6 1 7" > /proc/sys/kernel/printk

# insmod & mknod
insmod fpga_push_switch_driver.ko
insmod device_driver/huins.ko
mknod /dev/huins_driver c 242 0
mknod /dev/fpga_push_switch_driver c 265 0
chomd 777 /dev/huins_driver
