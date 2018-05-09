#!/bin/zsh

#go into workspace, make robot go take photos
#cd ~/abrar_ws/
#source devel/setup.bash
#rosrun finalProject main

#move photos into darknet data folder
#mv -v ~/cs309_ws/* ~/darknet/data/photos

#run YOLO on all photos
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/10.jpg |& tee result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/1.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/2.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/3.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/4.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/5.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/6.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/7.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/8.jpg |& tee -a result.txt
./darknet detect cfg/yolov3.cfg yolov3.weights data/photos/9.jpg |& tee -a result.txt
