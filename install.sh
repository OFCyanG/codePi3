sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 9DA31620334BD75D9DCB49F368818C72E52529D4
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/4.0 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-4.0.list
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libcurl3
sudo apt-get install -y mongodb-org

mongod --version
mongo --version

sudo dd if=/dev/zero of=/swapfile.img bs=1M count=2048
sudo mkswap /swapfile.img
sudo swapon /swapfile.img
cat /proc/swaps

sudo service mongod start

sudo apt-get install wpasupplicant wire-tools ifupdown
