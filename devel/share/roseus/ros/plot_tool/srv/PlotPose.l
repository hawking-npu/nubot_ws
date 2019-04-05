;; Auto-generated. Do not edit!


(when (boundp 'plot_tool::PlotPose)
  (if (not (find-package "PLOT_TOOL"))
    (make-package "PLOT_TOOL"))
  (shadow 'PlotPose (find-package "PLOT_TOOL")))
(unless (find-package "PLOT_TOOL::PLOTPOSE")
  (make-package "PLOT_TOOL::PLOTPOSE"))
(unless (find-package "PLOT_TOOL::PLOTPOSEREQUEST")
  (make-package "PLOT_TOOL::PLOTPOSEREQUEST"))
(unless (find-package "PLOT_TOOL::PLOTPOSERESPONSE")
  (make-package "PLOT_TOOL::PLOTPOSERESPONSE"))

(in-package "ROS")

(if (not (find-package "GEOMETRY_MSGS"))
  (ros::roseus-add-msgs "geometry_msgs"))




(defclass plot_tool::PlotPoseRequest
  :super ros::object
  :slots (_msg _series _append _symbol _symbol_size ))

(defmethod plot_tool::PlotPoseRequest
  (:init
   (&key
    ((:msg __msg) (instance geometry_msgs::Pose :init))
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
    ;; geometry_msgs/Pose _msg
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
     ;; geometry_msgs/Pose _msg
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
   ;; geometry_msgs/Pose _msg
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

(defclass plot_tool::PlotPoseResponse
  :super ros::object
  :slots ())

(defmethod plot_tool::PlotPoseResponse
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

(defclass plot_tool::PlotPose
  :super ros::object
  :slots ())

(setf (get plot_tool::PlotPose :md5sum-) "0b57137281cf86b445cc0e3821c92f9e")
(setf (get plot_tool::PlotPose :datatype-) "plot_tool/PlotPose")
(setf (get plot_tool::PlotPose :request) plot_tool::PlotPoseRequest)
(setf (get plot_tool::PlotPose :response) plot_tool::PlotPoseResponse)

(defmethod plot_tool::PlotPoseRequest
  (:response () (instance plot_tool::PlotPoseResponse :init)))

(setf (get plot_tool::PlotPoseRequest :md5sum-) "0b57137281cf86b445cc0e3821c92f9e")
(setf (get plot_tool::PlotPoseRequest :datatype-) "plot_tool/PlotPoseRequest")
(setf (get plot_tool::PlotPoseRequest :definition-)
      "
geometry_msgs/Pose msg
uint32 series
bool append
char symbol
uint32 symbol_size

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

(setf (get plot_tool::PlotPoseResponse :md5sum-) "0b57137281cf86b445cc0e3821c92f9e")
(setf (get plot_tool::PlotPoseResponse :datatype-) "plot_tool/PlotPoseResponse")
(setf (get plot_tool::PlotPoseResponse :definition-)
      "
geometry_msgs/Pose msg
uint32 series
bool append
char symbol
uint32 symbol_size

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



(provide :plot_tool/PlotPose "0b57137281cf86b445cc0e3821c92f9e")


