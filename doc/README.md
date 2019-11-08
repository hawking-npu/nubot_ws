# README

## 环境配置

1. 安装Ubuntu 16.04

2. 安装ROS kinetic, 参考[ROS wiki](http://wiki.ros.org/kinetic/Installation/Ubuntu)

3. 安装qt-ros-plugin, 注意这里的qt版本是5.9

   `$ sudo add-apt-repository ppa:levi-armstrong/qt-libraries-xenial`

   `$ sudo add-apt-repository ppa:levi-armstrong/ppa`

   `$ sudo apt-get update`

   `$ sudo apt-get install qt57creator-plugin-ros`

4. 为qt-ros-plugin加载ROS变量: 更改`/usr/share/applications/qt59creator.desktop`文件, 在Exec行的等号后添加`bash -i -c -h`

5. 设置全景摄像头, [tiscamera](https://github.com/TheImagingSource/tiscamera)

   **以下安装代码仅供参考, 实际以tiscamera的文档为主**

   `$ cd ~/`

   `$ git clone https://github.com/TheImagingSource/tiscamera.git`

   `$ cd tiscamera`

   `$ sudo ./scripts/install-dependencies.sh --compilation --runtime`

   `$ sudo ./scripts/install-dependencies.sh --runtime`

   `$ mkdir build`

   `$ cd build`

   `$ cmake -DBUILD_ARAVIS=ON -DBUILD_TOOLS=ON ..`

   `$ make`

   `$ sudo make install`

   `$ sudo systemctl daemon-reload                 # make systemd aware of gige-daemon`
   
   `$ sudo systemctl enable gige-daemon.service    # start on every boot`
   
   `$ sudo systemctl start gige-daemon.service     # start the actual daemon`
   
   `$ sudo systemctl status gige-daemon.service    # check if statemd say everything is ok`

6. ssh设置

   `$ sudo apt-get install openssh-server`

7. 相关依赖包的安装

   `$ sudo apt-get install ros-kinetic-serial`

   `$ sudo apt-get install ros-kinetic-driver-base`

   `$ sudo apt-get install ros-kinetic-teleop-twist-keyboard`

8. 串口设置

   在`/etc/udev/rules.d`中新建`70-ttyusb.rules`文件,在其中输入

   `KERNEL=="ttyUSB[0-9]*",MODE="0666"`, 然后在终端中输入

   `$ sudo chmod 666 70-ttyusb.rules`

9. 锁屏设置(可选)

   将`/etc/systemd/logind.conf`文件中的`HandleLidSwitch=suspend`修改为`HandleLidSwitch=ignore`, 然后执行

   `sudo systemctl restart systemd-logind`

10. 代码相关设置

    **将根目录放在`~/`下**

    `$ cd ~/nubot_ws`

    `$ sudo chmod +x configure`

    `$ sudo ./configure`

    `$ catkin_make`

    `$ sudo echo 'source ~/nubot_ws/devel/setup.bash' >> ~/.bashrc`

    `$ sudo echo 'export AGENT=2' >> ~/.bashrc`

    `$ source ~/.bashrc`

    `$ sudo chmod +x ~/nubot_ws/src/camera_driver/tiscamera/tis_camera/scripts/gige_camera_node.py`

    `$ sudo chmod +x ~/nubot_ws/src/nubot/nubot_hwcontroller/scripts/twist_keyboard.py`

## 编译

`$ catkin_make`

## 运行

`$ roslaunch nubot_common nubot.launch`

`$ roslaunch nubot_control nubot_control.launch`

## 其他

可以使用qtcreator-ros打开nubot_ws.workspace, 也可以查看ROS_Qt.doc进行相关配置
