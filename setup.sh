# give printk enough permission
echo "7 6 1 7" > /proc/sys/kernel/printk

# insmod & mknod
insmod device_driver/huins.ko
mknod /dev/huins_driver c 242 0
chomd 777 /dev/huins_driver
