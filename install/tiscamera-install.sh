cd ~/
git clone https://github.com/TheImagingSource/tiscamera.git
cd tiscamera
sudo ./scripts/install-dependencies.sh --compilation --runtime
sudo ./scripts/install-dependencies.sh --runtime
mkdir build
cd build
cmake -DBUILD_ARAVIS=ON -DBUILD_TOOLS=ON ..
make
sudo make install
# 可选
sudo systemctl daemon-reload                 # make systemd aware of gige-daemon
sudo systemctl enable gige-daemon.service    # start on every boot
sudo systemctl start gige-daemon.service     # start the actual daemon
sudo systemctl status gige-daemon.service    # check if statemd say everything is ok
