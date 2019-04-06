#!/usr/bin/env python
import rospy
from std_msgs.msg import Int8MultiArray
from geometry_msgs.msg import Twist

rospy.init_node ('scared_robot', anonymous=True)
pub = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size = 1)

def callback(data):
#	rospy.loginfo(rospy.get_caller_id() + 'proximity')
    #rospy.loginfo('%d',  data.data[0])
    left = data.data[0]
    right = data.data[5]
    frontLeft = data.data[2]
    frontRight = data.data[3]
    vel_msg = Twist()
    vel_msg.linear.x = 2
    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    vel_msg.angular.z = 0
    
    if checkFront(frontLeft,  frontRight):          # check if front is too close
        rospy.loginfo(rospy.get_caller_id() + ' Front too close! %d, %d',  frontLeft,  frontRight)
        vel_msg.linear.x = 0                              #slow down
#        if left > right and left > 3:                                            # check if left is closer than right, turn right
#            vel_msg.angular.z = -1
#        elif right > left and right > 3:                                          #check if right is closer than left, turn left
#            vel_msg.angular.z = 1
#        else:                                                           #sides are equidistant
        if frontLeft >= frontRight:                         #check which side infront is closer, turn away
            vel_msg.angular.z = -1
        elif frontRight > frontLeft:
            vel_msg.angular.z = 1
    else:                                                           #if nothing in front, check sides
        if left > right:
            if left > 7:
                vel_msg.angular.z = -1
            elif left > 4:
                vel_msg.angular.z = 1
        elif right > left:
            if right > 7:
                vel_msg.angular.z = 1
            elif right > 4:
                vel_msg.angular.z = -1
    
    pub.publish(vel_msg)


def checkFront( frontLeft,  frontRight):
    if frontLeft > 8 or frontRight > 8:
        return 1;
    return 0;


def listener():
	rospy.Subscriber('proximity', Int8MultiArray, callback)
	rospy.spin()

if __name__ == '__main__':
    rospy.loginfo(rospy.get_caller_id() + ' Started')
    listener()
