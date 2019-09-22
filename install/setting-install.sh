sudo apt-get install ros-kinetic-serial
sudo apt-get install ros-kinetic-driver-base

#U转串
sudo echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >> /etc/udev/rules.d/70-ttyusb.rules
sudo chmod 666 /etc/udev/rules.d/70-ttyusb.rules

sudo echo 'KERNEL=="ttyUSB*", ATTRS{idVendor}=="1a86", ATTRS{idProduct}=="7523", MODE:="0666", SYMLINK+="Robot_control"' >> /etc/udev/rules.d/Robot_control.rules
sudo chmod 666 /etc/udev/rules.d/Robot_control.rules

sudo echo 'KERNEL=="ttyUSB*", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", MODE:="0666", SYMLINK+="HI_control"' >> /etc/udev/rules.d/HI_control.rules
sudo chmod 666 /etc/udev/rules.d/HI_control.rules
