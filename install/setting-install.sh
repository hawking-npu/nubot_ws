sudo apt-get install ros-kinetic-serial
sudo apt-get install ros-kinetic-driver-base

#U转串
sudo echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >> /etc/udev/rules.d/70-ttyusb.rules
sudo chmod 666 /etc/udev/rules.d/70-ttyusb.rules
