; Auto-generated. Do not edit!


(cl:in-package plot_tool-srv)


;//! \htmlinclude PlotPath-request.msg.html

(cl:defclass <PlotPath-request> (roslisp-msg-protocol:ros-message)
  ((msg
    :reader msg
    :initarg :msg
    :type nav_msgs-msg:Path
    :initform (cl:make-instance 'nav_msgs-msg:Path))
   (series
    :reader series
    :initarg :series
    :type cl:integer
    :initform 0)
   (append
    :reader append
    :initarg :append
    :type cl:boolean
    :initform cl:nil)
   (symbol
    :reader symbol
    :initarg :symbol
    :type cl:integer
    :initform 0)
   (symbol_size
    :reader symbol_size
    :initarg :symbol_size
    :type cl:integer
    :initform 0))
)

(cl:defclass PlotPath-request (<PlotPath-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PlotPath-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PlotPath-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name plot_tool-srv:<PlotPath-request> is deprecated: use plot_tool-srv:PlotPath-request instead.")))

(cl:ensure-generic-function 'msg-val :lambda-list '(m))
(cl:defmethod msg-val ((m <PlotPath-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:msg-val is deprecated.  Use plot_tool-srv:msg instead.")
  (msg m))

(cl:ensure-generic-function 'series-val :lambda-list '(m))
(cl:defmethod series-val ((m <PlotPath-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:series-val is deprecated.  Use plot_tool-srv:series instead.")
  (series m))

(cl:ensure-generic-function 'append-val :lambda-list '(m))
(cl:defmethod append-val ((m <PlotPath-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:append-val is deprecated.  Use plot_tool-srv:append instead.")
  (append m))

(cl:ensure-generic-function 'symbol-val :lambda-list '(m))
(cl:defmethod symbol-val ((m <PlotPath-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:symbol-val is deprecated.  Use plot_tool-srv:symbol instead.")
  (symbol m))

(cl:ensure-generic-function 'symbol_size-val :lambda-list '(m))
(cl:defmethod symbol_size-val ((m <PlotPath-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:symbol_size-val is deprecated.  Use plot_tool-srv:symbol_size instead.")
  (symbol_size m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PlotPath-request>) ostream)
  "Serializes a message object of type '<PlotPath-request>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'msg) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'series)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'series)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'series)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'series)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'append) 1 0)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'symbol)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'symbol_size)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'symbol_size)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'symbol_size)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'symbol_size)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PlotPath-request>) istream)
  "Deserializes a message object of type '<PlotPath-request>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'msg) istream)
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'series)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'series)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'series)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'series)) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'append) (cl:not (cl:zerop (cl:read-byte istream))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'symbol)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'symbol_size)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'symbol_size)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'symbol_size)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'symbol_size)) (cl:read-byte istream))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PlotPath-request>)))
  "Returns string type for a service object of type '<PlotPath-request>"
  "plot_tool/PlotPathRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPath-request)))
  "Returns string type for a service object of type 'PlotPath-request"
  "plot_tool/PlotPathRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PlotPath-request>)))
  "Returns md5sum for a message object of type '<PlotPath-request>"
  "11f01eca6b922e1051e9a725443a07ad")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PlotPath-request)))
  "Returns md5sum for a message object of type 'PlotPath-request"
  "11f01eca6b922e1051e9a725443a07ad")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PlotPath-request>)))
  "Returns full string definition for message of type '<PlotPath-request>"
  (cl:format cl:nil "~%nav_msgs/Path msg~%uint32 series~%bool append~%char symbol~%uint32 symbol_size~%~%================================================================================~%MSG: nav_msgs/Path~%#An array of poses that represents a Path for a robot to follow~%Header header~%geometry_msgs/PoseStamped[] poses~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PlotPath-request)))
  "Returns full string definition for message of type 'PlotPath-request"
  (cl:format cl:nil "~%nav_msgs/Path msg~%uint32 series~%bool append~%char symbol~%uint32 symbol_size~%~%================================================================================~%MSG: nav_msgs/Path~%#An array of poses that represents a Path for a robot to follow~%Header header~%geometry_msgs/PoseStamped[] poses~%~%================================================================================~%MSG: std_msgs/Header~%# Standard metadata for higher-level stamped data types.~%# This is generally used to communicate timestamped data ~%# in a particular coordinate frame.~%# ~%# sequence ID: consecutively increasing ID ~%uint32 seq~%#Two-integer timestamp that is expressed as:~%# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')~%# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')~%# time-handling sugar is provided by the client library~%time stamp~%#Frame this data is associated with~%# 0: no frame~%# 1: global frame~%string frame_id~%~%================================================================================~%MSG: geometry_msgs/PoseStamped~%# A Pose with reference coordinate frame and timestamp~%Header header~%Pose pose~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PlotPath-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'msg))
     4
     1
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PlotPath-request>))
  "Converts a ROS message object to a list"
  (cl:list 'PlotPath-request
    (cl:cons ':msg (msg msg))
    (cl:cons ':series (series msg))
    (cl:cons ':append (append msg))
    (cl:cons ':symbol (symbol msg))
    (cl:cons ':symbol_size (symbol_size msg))
))
;//! \htmlinclude PlotPath-response.msg.html

(cl:defclass <PlotPath-response> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass PlotPath-response (<PlotPath-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PlotPath-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PlotPath-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name plot_tool-srv:<PlotPath-response> is deprecated: use plot_tool-srv:PlotPath-response instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PlotPath-response>) ostream)
  "Serializes a message object of type '<PlotPath-response>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PlotPath-response>) istream)
  "Deserializes a message object of type '<PlotPath-response>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PlotPath-response>)))
  "Returns string type for a service object of type '<PlotPath-response>"
  "plot_tool/PlotPathResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPath-response)))
  "Returns string type for a service object of type 'PlotPath-response"
  "plot_tool/PlotPathResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PlotPath-response>)))
  "Returns md5sum for a message object of type '<PlotPath-response>"
  "11f01eca6b922e1051e9a725443a07ad")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PlotPath-response)))
  "Returns md5sum for a message object of type 'PlotPath-response"
  "11f01eca6b922e1051e9a725443a07ad")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PlotPath-response>)))
  "Returns full string definition for message of type '<PlotPath-response>"
  (cl:format cl:nil "~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PlotPath-response)))
  "Returns full string definition for message of type 'PlotPath-response"
  (cl:format cl:nil "~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PlotPath-response>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PlotPath-response>))
  "Converts a ROS message object to a list"
  (cl:list 'PlotPath-response
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'PlotPath)))
  'PlotPath-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'PlotPath)))
  'PlotPath-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPath)))
  "Returns string type for a service object of type '<PlotPath>"
  "plot_tool/PlotPath")