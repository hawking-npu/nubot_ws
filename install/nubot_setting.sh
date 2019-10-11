cd ~/nubot_ws/
catkin_make
sudo echo 'source ~/nubot_ws/devel/setup.bash' >> ~/.bashrc
sudo echo 'export AGENT=1' >> ~/.bashrc

sudo chmod +x ~/gige
sudo chmod +x ~/keyboard
