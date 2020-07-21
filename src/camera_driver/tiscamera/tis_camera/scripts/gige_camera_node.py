#!/usr/bin/env python2
import gi
gi.require_version("Gst", "1.0")
gi.require_version("Tcam", "0.1")

from gi.repository import Tcam, Gst, GLib, GObject
import os
import subprocess
from collections import namedtuple

import rospy

DeviceInfo = namedtuple("DeviceInfo", "status name identifier connection_type")
CameraProperty = namedtuple(
    "CameraProperty", "status value min max default step type flags category group")

# Disable pylint false positives
# pylint:disable=E0712


class Camera:
    """"""

    def __init__(self, serial, width, height, framerate, color, liveview, topic_name=None, node_name=None):
        """ Constructor.
        Creates the sink pipeline and the source pipeline.

        :param serial: Serial number of the camera to use.
        :param width: Width of the video format, e.g. 640, 1920 etc,
        :param height: Height of the video format, e.g. 480, 1080
        :param framerate: Numerator of the frame rate, e.g. 15, 30, 60 etc
        :param color: If True, color is used, else gray scale
        :param liveview: If True an own live window is opened.
        :param topic_name: The ROS topic name for this Camera instance
        """
        Gst.init([])
        self.height = height
        self.width = width
        self.sample = None
        self.samplelocked = False
        self.newsample = False
        self.topic_name = "/camera/image_raw:=/"
        self.topic_name += topic_name if topic_name is not None else "tiscamera/image_raw"
        self.node_name = "__name:="
        self.node_name += node_name if node_name is not None else "gige_camera_node"

        print("self.topic_name: " + str(self.topic_name)) # /camera/image_raw:=/tiscamera/image_raw
        print("self.node_name: " + str(self.node_name))   # __name:=gige_camera_node
        self.pid = -1

        self.__remove_tmp_file()

        pixelformat = "BGRx"
        if not color:
            pixelformat = "GRAY8"

        if liveview:
            p = 'tcambin serial="%s" name=source ! video/x-raw,format=%s,width=%d,height=%d,framerate=%d/1' % (
                serial, pixelformat, width, height, framerate,)
            p += ' ! tee name=t'
            p += ' t. ! queue ! videoconvert ! video/x-raw,format=RGB ,width=%d,height=%d,framerate=%d/1! shmsink socket-path=/tmp/ros_mem' % (
                width, height, framerate,)
            p += ' t. ! queue ! videoconvert ! ximagesink'
        else:
            p = 'tcambin serial="%s" name=source ! video/x-raw,format=%s,width=%d,height=%d,framerate=%d/1' % (
                serial, pixelformat, width, height, framerate,)
            p += ' ! videoconvert ! video/x-raw,format=RGB ,width=%d,height=%d,framerate=%d/1! shmsink socket-path=/tmp/ros_mem' % (
                width, height, framerate,)

#        print(p)

        try:
            self.pipeline = Gst.parse_launch(p)
        except GLib.Error as error:
            raise RuntimeError("Error creating pipeline: {0}".format(error))

        self.pipeline.set_state(Gst.State.READY)
        if self.pipeline.get_state(10 * Gst.SECOND)[0] != Gst.StateChangeReturn.SUCCESS:
            raise RuntimeError("Failed to start video stream.")
        # Query a pointer to our source, so we can set properties.
        self.source = self.pipeline.get_by_name("source")

        # Create gscam_config variable with content
        gscam = 'shmsrc socket-path=/tmp/ros_mem ! video/x-raw-rgb, width=%d,height=%d,framerate=%d/1' % (
            width, height, framerate,)
        gscam += ',bpp=24,depth=24,blue_mask=16711680,green_mask=65280,red_mask=255 ! ffmpegcolorspace'
        os.environ["GSCAM_CONFIG"] = gscam

    def start_pipeline(self):
        """ Starts the camera sink pipeline and the rosrun process

        :return:
        """
        print("name:"+self.topic_name)
        print(self.node_name)
        try:
            self.pipeline.set_state(Gst.State.PLAYING)
            self.pid = subprocess.Popen(
                ["rosrun", "gscam", "gscam" , self.topic_name, self.node_name])
            input()

        except GLib.Error as error:
            print("Error starting pipeline: {0}".format(error))
            raise

    def stop_pipeline(self):
        """ Stops the camera pipeline. Should also kill the rosrun process, but is not implemented

        :return:
        """
        self.pipeline.set_state(Gst.State.PAUSED)
        self.pipeline.set_state(Gst.State.READY)
        self.pipeline.set_state(Gst.State.NULL)
        self.pid.kill()

    def list_properties(self):
        """ Helper function. List available properties

        :return:
        """
        for name in self.source.get_tcam_property_names():
            print(name)

    def get_property(self, property_name):
        """ Return the value of the passed property.

        Use list_properties for querying names of available properties.

        :param property_name: Name of the property, e.g. Gain, Exposure, Gain Auto.
        :return: Current value of the property.
        """
        try:
            return CameraProperty(*self.source.get_tcam_property(property_name))
        except GLib.Error as error:
            raise RuntimeError(
                "Error get Property {0}: {1}", property_name, format(error))

    def set_property(self, property_name, value):
        """ Set a property. Use list_properties for querying names of available properties.

        :param property_name: Name of the property, e.g. Gain, Exposure, Gain Auto.
        :param value: Value to be set.
        :return:
        """
        try:
            self.source.set_tcam_property(property_name, value)
        except GLib.Error as error:
            raise RuntimeError(
                "Error set Property {0}: {1}", property_name, format(error))

    def push_property(self, property_name):
        """ Simplify push properties, like Auto Focus one push

        :param property_name: Name of the property to be pushed
        :return:
        """
        try:
            self.source.set_tcam_property(property_name, True)

        except GLib.Error as error:
            raise RuntimeError(
                "Error set Property {0}: {1}", property_name, format(error))

    def __remove_tmp_file(self):
        """ Delete the memory file used by the pipelines to share memory

        :return:
        """
        try:
            os.remove('/tmp/ros_mem')
        except OSError:
            pass

def main():
    # Open the camera. Parameters are serial number, width, height, frame rate, color and liveview.
    cam = Camera("24514123", 640, 480, 30, True, False)
    #cam = Camera("35814515", 640, 480, 30, True, False)
    #cam = Camera("35814519", 640, 480, 30, True, False)

    # Set some properties
    cam.set_property("Exposure Auto", True)
    cam.set_property("Gain Auto", True)
    cam.set_property("Brightness Reference", 128)

    # Start the live stream from the camera and also "rosrun"
    cam.start_pipeline()

#    pid = os.fork();
#    if(pid==0):
#        if(os.execlp("rosrun", "rosrun", "gscam", "gscam", cam.topic_name, cam.node_name) < 0):
#          ROS_WARN("Process Gscam not found!")
#        return

    # Stop the camera pipeline.
    cam.stop_pipeline()

if __name__ == '__main__':
    main()

