#!/usr/bin/env python3

import rospy
#from sensor_msgs.msg import LaserScan
from nav_msgs.msg import OccupancyGrid
import sensor_msgs.msg
import tf
import numpy as np
import matplotlib.pyplot as plt
import csv
import os

def home():
    return os.path.expanduser("~")

def grid_callback(data):
    obstical_coords = []
    print(data.info.width)
    print(data.info.height)
    for width in range(data.info.width - 1):
        for height in range(data.info.height - 1):
            if data.data[height * data.info.width + width] == 100:
                x = width * data.info.resolution + data.info.resolution / 2
                y = height * data.info.resolution + data.info.resolution / 2
                obstical_coords.append([x,y])
    #write(obstical_coords)
   
def main():
    rospy.init_node('grid_test')
    grid_sub = rospy.Subscriber('/map', OccupancyGrid, grid_callback)
    rospy.spin()

def write(data):
    with open(str(home()) + '/grid.csv', 'w') as f:
        csvwriter = csv.writer(f)
        for k in data:
            line = [k[0], k[1]]
            csvwriter.writerow(line)
            

def plot(data):
    x = np.array([])
    y = np.array([])
    for k in data:
        x = np.append(x, [k[0]])
        y = np.append(y, [k[1]])
    plt.scatter(x,y)


    



if __name__ == '__main__':
    main()
       

