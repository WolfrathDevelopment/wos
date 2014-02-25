#! /bin/bash
cd wOS_Revised
cd src
make clean
make
cd ..
sudo bash update_image.sh
bash run_bochs.sh
