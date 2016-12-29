#sudo docker build . -t helvede-dev
sudo docker run -v `pwd`/output:/root/helvede/output -v `pwd`/source:/root/helvede/source -w /root/helvede helvede-dev /bin/bash -c "./build.sh"
