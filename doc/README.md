# 轮式机器人软件组环境配置文档

## 前言

本文档将持续记录并更新轮式机器人软件组的环境配置，以期为后来者们提供帮助，快速上手轮式足球机器人软件系统。

对于文本中出现的命令，除了按照步骤做了还需要你们都了解它们各自的作用。

本文档整理于nubot以及tiscanera的各个文档，[点我进行一览](http://git.codesdream.com/yws/nwpu_hawking_doc)。



## 安装Ubuntu

安装版本ubuntu16.04，安装步骤请自行参考百度。

[安装完成后，请将软件源配置为国内的。](https://blog.csdn.net/weixin_41043145/article/details/92167474)
* i advice you to change source to tsinghua university sourc or ali source.

在软件源配置过程中可能会出现等待时长过久的问题，请耐心等待。



## 安装ros

[请在ros的wiki官方文档中寻找安装指导。](http://wiki.ros.org/cn/kinetic/Installation/Ubuntu)

需要注意的是，在安装时请选择国内或者新加坡的软件源，将会大大提高下载速度，这在文档中有所提示，请留意。

安装版本请选择桌面完整版，同时不要忘记初始化rosdep和配置环境。

when you are installing ros,you may meet the error list below:
```
sudo rosdep init
ERROR: cannot download default sources list from:
https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/sources.list.d/20-default.list
Website may be down.
```
i solve this problem by use vpn
when you execute `rosdep update`,you may meet problems below:
```
reading in sources list data from /etc/ros/rosdep/sources.list.d
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/osx-homebrew.yaml]:
	<urlopen error [Errno -2] Name or service not known> (https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/osx-homebrew.yaml)
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/base.yaml]:
	<urlopen error [Errno -2] Name or service not known> (https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/base.yaml)
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/python.yaml]:
	<urlopen error [Errno -2] Name or service not known> (https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/python.yaml)
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/ruby.yaml]:
	<urlopen error [Errno -2] Name or service not known> (https://raw.githubusercontent.com/ros/rosdistro/master/rosdep/ruby.yaml)
ERROR: unable to process source [https://raw.githubusercontent.com/ros/rosdistro/master/releases/fuerte.yaml]:
	Failed to download target platform data for gbpdistro:
	<urlopen error [Errno -2] Name or service not known>
Query rosdistro index https://raw.githubusercontent.com/ros/rosdistro/master/index-v4.yaml
ERROR: error loading sources list:
	<urlopen error <urlopen error [Errno -2] Name or service not known> (https://raw.githubusercontent.com/ros/rosdistro/master/index
```

i solve this problem by modify hosts:
`sudo gedit /etc/hosts`
add context below in file hosts:
```
151.101.84.133  raw.githubusercontent.com
140.82.113.4        github.com
185.199.111.153     assets-cdn.github.com
199.232.69.194      github.global.ssl.fastly.net
```

 

在安装完成后，补充安装以下包：

```
sudo apt-get install ros-kinetic-serial
sudo apt-get install ros-kinetic-driver-base
sudo apt-get install ros-kinetic-teleop-twist-keyboard
```



## 安装opencv

在基地的硬盘中有opencv的相关软件，路径为

```
D:\软件分区\软件\opencv\3.2-release\Ubuntu
```

解压后将lib文件夹复制到usr/local下，同时配置环境：

```
sudo echo 'PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig' >> ~/.bashrc
```


about install opencv, you can refer to [install opencv](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)
```

Next Tutorial: Using OpenCV with gcc and CMake

The following steps have been tested for Ubuntu 10.04 but should work with other distros as well.
Required Packages

    GCC 4.4.x or later
    CMake 2.8.7 or higher
    Git
    GTK+2.x or higher, including headers (libgtk2.0-dev)
    pkg-config
    Python 2.6 or later and Numpy 1.5 or later with developer packages (python-dev, python-numpy)
    ffmpeg or libav development packages: libavcodec-dev, libavformat-dev, libswscale-dev
    [optional] libtbb2 libtbb-dev
    [optional] libdc1394 2.x
    [optional] libjpeg-dev, libpng-dev, libtiff-dev, libjasper-dev, libdc1394-22-dev
    [optional] CUDA Toolkit 6.5 or higher

The packages can be installed using a terminal and the following commands or by using Synaptic Manager:
[compiler] sudo apt-get install build-essential
[required] sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
[optional] sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
Getting OpenCV Source Code

You can use the latest stable OpenCV version or you can grab the latest snapshot from our Git repository.
Getting the Latest Stable OpenCV Version

    Go to our downloads page.
    Download the source archive and unpack it.

Getting the Cutting-edge OpenCV from the Git Repository

Launch Git client and clone OpenCV repository. If you need modules from OpenCV contrib repository then clone it as well.

For example
cd ~/<my_working_directory>
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
Building OpenCV from Source Using CMake

    Create a temporary directory, which we denote as <cmake_build_dir>, where you want to put the generated Makefiles, project files as well the object files and output binaries and enter there.

    For example
    cd ~/opencv
    mkdir build
    cd build

    Configuring. Run cmake [<some optional parameters>] <path to the OpenCV source directory>

    For example
    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..

    or cmake-gui
        set full path to OpenCV source code, e.g. /home/user/opencv
        set full path to <cmake_build_dir>, e.g. /home/user/opencv/build
        set optional parameters
        run: “Configure”
        run: “Generate”

    Note
        Use cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local .. , without spaces after -D if the above example doesn't work.

    Description of some parameters
        build type: CMAKE_BUILD_TYPE=Release\Debug
        to build with modules from opencv_contrib set OPENCV_EXTRA_MODULES_PATH to <path to opencv_contrib/modules/>
        set BUILD_DOCS for building documents
        set BUILD_EXAMPLES to build all examples
    [optional] Building python. Set the following python parameters:
        PYTHON2(3)_EXECUTABLE = <path to python>
        PYTHON_INCLUDE_DIR = /usr/include/python<version>
        PYTHON_INCLUDE_DIR2 = /usr/include/x86_64-linux-gnu/python<version>
        PYTHON_LIBRARY = /usr/lib/x86_64-linux-gnu/libpython<version>.so
        PYTHON2(3)_NUMPY_INCLUDE_DIRS = /usr/lib/python<version>/dist-packages/numpy/core/include/
    [optional] Building java.
        Unset parameter: BUILD_SHARED_LIBS
        It is useful also to unset BUILD_EXAMPLES, BUILD_TESTS, BUILD_PERF_TESTS - as they all will be statically linked with OpenCV and can take a lot of memory.
    [optional] Generate pkg-config info
        Add this flag when running CMake: -DOPENCV_GENERATE_PKGCONFIG=ON
        Will generate the .pc file for pkg-config and install it.
        Useful if not using CMake in projects that use OpenCV
        Installed as opencv4, usage: pkg-config --cflags --libs opencv4

    Build. From build directory execute make, it is recommended to do this in several threads

    For example
    make -j7 # runs 7 jobs in parallel

    [optional] Building documents. Enter <cmake_build_dir/doc/> and run make with target "doxygen"

    For example
    cd ~/opencv/build/doc/
    make -j7 doxygen
    To install libraries, execute the following command from build directory
    sudo make install

    [optional] Running tests
        Get the required test data from OpenCV extra repository.

    For example
    git clone https://github.com/opencv/opencv_extra.git
        set OPENCV_TEST_DATA_PATH environment variable to <path to opencv_extra/testdata>.
        execute tests from build directory.

    For example
    <cmake_build_dir>/bin/opencv_test_core
```


when you execute `cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..`
you may suspend at 
```
-- Looking for sys/videoio.h
-- Looking for sys/videoio.h - not found
-- Checking for modules 'libavcodec;libavformat;libavutil;libswscale'
--   Found libavcodec, version 56.60.100
--   Found libavformat, version 56.40.101
--   Found libavutil, version 54.31.100
--   Found libswscale, version 3.1.101
-- Checking for module 'libavresample'
--   No package 'libavresample' found
-- IPPICV: Download: ippicv_2019_lnx_intel64_general_20180723.tgz
```

* you can solve this problem by:[solve](https://blog.csdn.net/orDream/article/details/84311697)

```
修改opencv里相关配置文件

    # 打开终端，输入
     
        gedit ~/DownLoad/opencv_source/opencv/3rdparty/ippicv/ippicv.cmake #就是这个文件的路径
     
    # 将47行的
     
         "https://raw.githubusercontent.com/opencv/opencv_3rdparty/${IPPICV_COMMIT}ippicv/"
     
     
     
    # 改为步骤1中手动下载的文件的本地路径(也就是将网络下载的模式改为本地文件下载的模式):
     
        "file:~/Downloads/" #（仅供参考，根据自己的路径填写）

编辑完成保存退出。

3，重新cmake

    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..

cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
```

* you need to edit environment variables:[edit environment](https://blog.csdn.net/zong596568821xp/article/details/80393810)
> 首先将OpenCV的库添加到路径，从而可以让系统找到

sudo gedit /etc/ld.so.conf.d/opencv.conf 

执行此命令后打开的可能是一个空白的文件，不用管，只需要在文件末尾添加

/usr/local/lib  

然后保存退出，执行如下命令使得刚才的配置路径生效

sudo ldconfig  

配置bash，执行如下命令

sudo gedit /etc/bash.bashrc  

在末尾添加

    PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig  
      
    export PKG_CONFIG_PATH  

保存退出，然后执行如下命令使得配置生效

> source /etc/bash.bashrc  



## 安装git

```
sudo apt-get install git
```



## 安装摄像头相关

按ctrl+T打开终端，这将使你位于home下，注意这是必要的，tiscamera以及nubot_ws工程文件都要直接放在home下，不要在home下新建其他的文件夹然后将工程文件放在这些文件夹中，这将使得工程文件无法被找到。

执行以下命令克隆目标仓库，注意，当遇到克隆速度缓慢或者是克隆总是提前结束并报错时，请尝试使用VPN或者是手机热点。

```
git clone https://github.com/TheImagingSource/tiscamera.git
```

克隆完成之后，执行以下命令，配置tiscamera：

```
cd tiscamera
sudo ./scripts/install-dependencies.sh --compilation --runtime
sudo ./scripts/install-dependencies.sh --runtime
mkdir build
cd build
cmake -DBUILD_ARAVIS=ON -DBUILD_TOOLS=ON ..
make
sudo make install
```

这些命令是可选的：

```
sudo systemctl daemon-reload                 # make systemd aware of gige-daemon
sudo systemctl enable gige-daemon.service    # start on every boot
sudo systemctl start gige-daemon.service     # start the actual daemon
sudo systemctl status gige-daemon.service    # check if statemd say everything is ok
```

配置tiscamera的过程参阅自[这里](https://github.com/TheImagingSource/tiscamera)。



## 配置U转串

```
sudo echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >> /etc/udev/rules.d/70-ttyusb.rules
sudo chmod 666 /etc/udev/rules.d/70-ttyusb.rules
```
when you execute command above,you may get error like:
```
sudo echo 'KERNEL=="ttyUSB[0-9]*",MODE="0666"' >> /etc/udev/rules.d/70-ttyusb.rules
bash: /etc/udev/rules.d/70-ttyusb.rules: Permission denied
```



## 配置机器人工程代码

这是我们的机器人的软件系统的代码，首先运行以下命令克隆到本地，注意事项与安装摄像头相关时相同。

```
git clone https://github.com/hawking-npu/nubot_ws.git
```

进行下列配置：

```
cd ~/nubot_ws
sudo chmod +x configure
sudo ./configure
catkin_make
sudo echo 'source ~/nubot_ws/devel/setup.bash' >> ~/.bashrc
sudo echo 'export AGENT=2' >> ~/.bashrc
source ~/.bashrc

# 这里bashrc实际上相当于Windows的环境变量文件，设置了环境变量后才能在命令行快捷使用我们的指令

sudo chmod +x ~/nubot_ws/src/camera_driver/tiscamera/tis_camera/scripts/gige_camera_node.py
sudo chmod +x ~/nubot_ws/src/nubot/nubot_hwcontroller/scripts/twist_keyboard.py
```

最后再次编译：

```
catkin_make
```



## 配置摄像头

首先执行：

```
ifconfig
```

找到本机网卡或者转接器的硬件名称（最开始的字段就是）。
运行：

```
sudo gedit  /etc/network/interfaces
```

将文件内容改为：

```
# interfaces(5) file used by ifup(8) and ifdown(8)

auto lo
iface lo inet loopback

auto enx503eaadf3bb1
iface enx503eaadf3bb1 inet static
address 192.168.1.100
netmask 255.255.255.0
gateway 192.168.1.1
dns-nameservers 8.8.4.4 8.8.8.8
```

其中 enx503eaadf3bb1 为 ifconfig 查看到的网口的编号。

完成后执行：

```
sudo /etc/init.d/networking restart
```

重启网卡后，执行：

```
tcam-ctrl -l
```

查看目前可用的设备，若正常显示，则记住设备的serial编号，到

```
/src/camera_driver/tiscamera/tis_camera/scripts/gige_camera_node.py
```

修改，在main()函数开头将对应编号的那一行取消注释；若找不到设备，尝试重新restart或者重启电脑，或者关掉wifi。

至此，摄像头已经被连接。

`gst-launch-1.0 tcambin ! videoconvert ! ximagesink`
reboot 

test if your opencv is ok:
cd /opencv/samples/cpp/example_cmake
cmake .
make
./opencv_example



## 运行

### 正常运行

```
roslaunch nubot_common nubot.launch

roslaunch nubot_control nubot_control.launch
```

### 运行特定部分

```
roslaunch nubot_hwcontrol nubot_hwcontrol.launch  #上下位机通信

roslaunch nubot_control nubot_control.launch      #策略算法

roslaunch nubot_hwcontrol twist_keyboard.py       #简单键盘控制

roslaunch omni_vision omni_vision.launch          #打开全景
```

#####    #这里建议自行学习launch文件的语法，以便调试

###  机器人运行配置

```
/nubot_ws/src/nubot/nubot_control/launch/nubot_control.launch  
#包括了策略中的比赛模式'mode_launch'以及机器人编号'role_launch'等 都在这里更改

~/.bashrc 文件中，AGENT也要同时更改为对应机器人编号
```



## 问题修复

若报错缺少python库gi，请直接apt-get安装。

## some declarations about tiscamera
in path ~/nubot_ws/src/camera_driver/tiscamera file `gscam` come from:
- [camera_umd](https://github.com/ros-drivers/camera_umd)
- [gscam](https://github.com/ros-drivers/gscam) and [tiscamera ROS example](https://github.com/TheImagingSource/tiscamera/tree/master/examples/ROS)
- [zed-ros-wrapper](https://github.com/stereolabs/zed-ros-wrapper) 



## 修改记录

@yws, 20200714

@shy, 20200716

@cjr, 20200721 

