;; Auto-generated. Do not edit!


(when (boundp 'plot_tool::PlotPath)
  (if (not (find-package "PLOT_TOOL"))
    (make-package "PLOT_TOOL"))
  (shadow 'PlotPath (find-package "PLOT_TOOL")))
(unless (find-package "PLOT_TOOL::PLOTPATH")
  (make-package "PLOT_TOOL::PLOTPATH"))
(unless (find-package "PLOT_TOOL::PLOTPATHREQUEST")
  (make-package "PLOT_TOOL::PLOTPATHREQUEST"))
(unless (find-package "PLOT_TOOL::PLOTPATHRESPONSE")
  (make-package "PLOT_TOOL::PLOTPATHRESPONSE"))

(in-package "ROS")

(if (not (find-package "NAV_MSGS"))
  (ros::roseus-add-msgs "nav_msgs"))




(defclass plot_tool::PlotPathRequest
  :super ros::object
  :slots (_msg _series _append _symbol _symbol_size ))

(defmethod plot_tool::PlotPathRequest
  (:init
   (&key
    ((:msg __msg) (instance nav_msgs::Path :init))
    ((:series __series) 0)
    ((:append __append) nil)
    ((:symbol __symbol) 0)
    ((:symbol_size __symbol_size) 0)
    )
   (send-super :init)
   (setq _msg __msg)
   (setq _series (round __series))
   (setq _append __append)
   (setq _symbol (round __symbol))
   (setq _symbol_size (round __symbol_size))
   self)
  (:msg
   (&rest __msg)
   (if (keywordp (car __msg))
       (send* _msg __msg)
     (progn
       (if __msg (setq _msg (car __msg)))
       _msg)))
  (:series
   (&optional __series)
   (if __series (setq _series __series)) _series)
  (:append
   (&optional __append)
   (if __append (setq _append __append)) _append)
  (:symbol
   (&optional __symbol)
   (if __symbol (setq _symbol __symbol)) _symbol)
  (:symbol_size
   (&optional __symbol_size)
   (if __symbol_size (setq _symbol_size __symbol_size)) _symbol_size)
  (:serialization-length
   ()
   (+
    ;; nav_msgs/Path _msg
    (send _msg :serialization-length)
    ;; uint32 _series
    4
    ;; bool _append
    1
    ;; char _symbol
    1
    ;; uint32 _symbol_size
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; nav_msgs/Path _msg
       (send _msg :serialize s)
     ;; uint32 _series
       (write-long _series s)
     ;; bool _append
       (if _append (write-byte -1 s) (write-byte 0 s))
     ;; char _symbol
       (write-byte _symbol s)
     ;; uint32 _symbol_size
       (write-long _symbol_size s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; nav_msgs/Path _msg
     (send _msg :deserialize buf ptr-) (incf ptr- (send _msg :serialization-length))
   ;; uint32 _series
     (setq _series (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; bool _append
     (setq _append (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;; char _symbol
     (setq _symbol (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; uint32 _symbol_size
     (setq _symbol_size (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(defclass plot_tool::PlotPathResponse
  :super ros::object
  :slots ())

(defmethod plot_tool::PlotPathResponse
  (:init
   (&key
    )
   (send-super :init)
   self)
  (:serialization-length
   ()
   (+
    0
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;;
   self)
  )

(defclass plot_tool::PlotPath
  :super ros::object
  :slots ())

(setf (get plot_tool::PlotPath :md5sum-) "11f01eca6b922e1051e9a725443a07ad")
(setf (get plot_tool::PlotPath :datatype-) "plot_tool/PlotPath")
(setf (get plot_tool::PlotPath :request) plot_tool::PlotPathRequest)
(setf (get plot_tool::PlotPath :response) plot_tool::PlotPathResponse)

(defmethod plot_tool::PlotPathRequest
  (:response () (instance plot_tool::PlotPathResponse :init)))

(setf (get plot_tool::PlotPathRequest :md5sum-) "11f01eca6b922e1051e9a725443a07ad")
(setf (get plot_tool::PlotPathRequest :datatype-) "plot_tool/PlotPathRequest")
(setf (get plot_tool::PlotPathRequest :definition-)
      "
nav_msgs/Path msg
uint32 series
bool append
char symbol
uint32 symbol_size

================================================================================
MSG: nav_msgs/Path
#An array of poses that represents a Path for a robot to follow
Header header
geometry_msgs/PoseStamped[] poses

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: geometry_msgs/PoseStamped
# A Pose with reference coordinate frame and timestamp
Header header
Pose pose

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
---


")

(setf (get plot_tool::PlotPathResponse :md5sum-) "11f01eca6b922e1051e9a725443a07ad")
(setf (get plot_tool::PlotPathResponse :datatype-) "plot_tool/PlotPathResponse")
(setf (get plot_tool::PlotPathResponse :definition-)
      "
nav_msgs/Path msg
uint32 series
bool append
char symbol
uint32 symbol_size

================================================================================
MSG: nav_msgs/Path
#An array of poses that represents a Path for a robot to follow
Header header
geometry_msgs/PoseStamped[] poses

================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
# 0: no frame
# 1: global frame
string frame_id

================================================================================
MSG: geometry_msgs/PoseStamped
# A Pose with reference coordinate frame and timestamp
Header header
Pose pose

================================================================================
MSG: geometry_msgs/Pose
# A representation of pose in free space, composed of position and orientation. 
Point position
Quaternion orientation

================================================================================
MSG: geometry_msgs/Point
# This contains the position of a point in free space
float64 x
float64 y
float64 z

================================================================================
MSG: geometry_msgs/Quaternion
# This represents an orientation in free space in quaternion form.

float64 x
float64 y
float64 z
float64 w
---


")



(provide :plot_tool/PlotPath "11f01eca6b922e1051e9a725443a07ad")


