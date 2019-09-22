#opencv
sudo cp -a ~/opencv/lib /usr/local

sudo echo 'PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig' >> ~/.bashrc
sudo echo 'export PKG_CONFIG_PATH' >> ~/.bashrc
