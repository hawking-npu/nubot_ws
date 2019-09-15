# 软件和更新--》确保你的"restricted"， "universe，" 和 "multiverse."前是打上勾的

# 更新
sudo apt-get update

# 添加源
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116

# 安装ROS
sudo apt-get install ros-kinetic-desktop-full

# 初始化ROS
sudo rosdep init
rosdep update

# 初始化ROS环境变量
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
