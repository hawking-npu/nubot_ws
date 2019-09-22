# 软件和更新--》确保你的"restricted"， "universe，" 和 "multiverse."前是打上勾的

# 更新
sudo apt-get update

# 添加源
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
# 安装ROS
sudo apt-get install ros-kinetic-desktop-full

# 初始化ROS
sudo rosdep init
rosdep update

# 初始化ROS环境变量
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
