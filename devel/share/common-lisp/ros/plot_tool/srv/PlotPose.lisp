; Auto-generated. Do not edit!


(cl:in-package plot_tool-srv)


;//! \htmlinclude PlotPose-request.msg.html

(cl:defclass <PlotPose-request> (roslisp-msg-protocol:ros-message)
  ((msg
    :reader msg
    :initarg :msg
    :type geometry_msgs-msg:Pose
    :initform (cl:make-instance 'geometry_msgs-msg:Pose))
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

(cl:defclass PlotPose-request (<PlotPose-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PlotPose-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PlotPose-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name plot_tool-srv:<PlotPose-request> is deprecated: use plot_tool-srv:PlotPose-request instead.")))

(cl:ensure-generic-function 'msg-val :lambda-list '(m))
(cl:defmethod msg-val ((m <PlotPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:msg-val is deprecated.  Use plot_tool-srv:msg instead.")
  (msg m))

(cl:ensure-generic-function 'series-val :lambda-list '(m))
(cl:defmethod series-val ((m <PlotPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:series-val is deprecated.  Use plot_tool-srv:series instead.")
  (series m))

(cl:ensure-generic-function 'append-val :lambda-list '(m))
(cl:defmethod append-val ((m <PlotPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:append-val is deprecated.  Use plot_tool-srv:append instead.")
  (append m))

(cl:ensure-generic-function 'symbol-val :lambda-list '(m))
(cl:defmethod symbol-val ((m <PlotPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:symbol-val is deprecated.  Use plot_tool-srv:symbol instead.")
  (symbol m))

(cl:ensure-generic-function 'symbol_size-val :lambda-list '(m))
(cl:defmethod symbol_size-val ((m <PlotPose-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader plot_tool-srv:symbol_size-val is deprecated.  Use plot_tool-srv:symbol_size instead.")
  (symbol_size m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PlotPose-request>) ostream)
  "Serializes a message object of type '<PlotPose-request>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PlotPose-request>) istream)
  "Deserializes a message object of type '<PlotPose-request>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PlotPose-request>)))
  "Returns string type for a service object of type '<PlotPose-request>"
  "plot_tool/PlotPoseRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPose-request)))
  "Returns string type for a service object of type 'PlotPose-request"
  "plot_tool/PlotPoseRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PlotPose-request>)))
  "Returns md5sum for a message object of type '<PlotPose-request>"
  "0b57137281cf86b445cc0e3821c92f9e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PlotPose-request)))
  "Returns md5sum for a message object of type 'PlotPose-request"
  "0b57137281cf86b445cc0e3821c92f9e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PlotPose-request>)))
  "Returns full string definition for message of type '<PlotPose-request>"
  (cl:format cl:nil "~%geometry_msgs/Pose msg~%uint32 series~%bool append~%char symbol~%uint32 symbol_size~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PlotPose-request)))
  "Returns full string definition for message of type 'PlotPose-request"
  (cl:format cl:nil "~%geometry_msgs/Pose msg~%uint32 series~%bool append~%char symbol~%uint32 symbol_size~%~%================================================================================~%MSG: geometry_msgs/Pose~%# A representation of pose in free space, composed of position and orientation. ~%Point position~%Quaternion orientation~%~%================================================================================~%MSG: geometry_msgs/Point~%# This contains the position of a point in free space~%float64 x~%float64 y~%float64 z~%~%================================================================================~%MSG: geometry_msgs/Quaternion~%# This represents an orientation in free space in quaternion form.~%~%float64 x~%float64 y~%float64 z~%float64 w~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PlotPose-request>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'msg))
     4
     1
     1
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PlotPose-request>))
  "Converts a ROS message object to a list"
  (cl:list 'PlotPose-request
    (cl:cons ':msg (msg msg))
    (cl:cons ':series (series msg))
    (cl:cons ':append (append msg))
    (cl:cons ':symbol (symbol msg))
    (cl:cons ':symbol_size (symbol_size msg))
))
;//! \htmlinclude PlotPose-response.msg.html

(cl:defclass <PlotPose-response> (roslisp-msg-protocol:ros-message)
  ()
)

(cl:defclass PlotPose-response (<PlotPose-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <PlotPose-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'PlotPose-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name plot_tool-srv:<PlotPose-response> is deprecated: use plot_tool-srv:PlotPose-response instead.")))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <PlotPose-response>) ostream)
  "Serializes a message object of type '<PlotPose-response>"
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <PlotPose-response>) istream)
  "Deserializes a message object of type '<PlotPose-response>"
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<PlotPose-response>)))
  "Returns string type for a service object of type '<PlotPose-response>"
  "plot_tool/PlotPoseResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPose-response)))
  "Returns string type for a service object of type 'PlotPose-response"
  "plot_tool/PlotPoseResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<PlotPose-response>)))
  "Returns md5sum for a message object of type '<PlotPose-response>"
  "0b57137281cf86b445cc0e3821c92f9e")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'PlotPose-response)))
  "Returns md5sum for a message object of type 'PlotPose-response"
  "0b57137281cf86b445cc0e3821c92f9e")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<PlotPose-response>)))
  "Returns full string definition for message of type '<PlotPose-response>"
  (cl:format cl:nil "~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'PlotPose-response)))
  "Returns full string definition for message of type 'PlotPose-response"
  (cl:format cl:nil "~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <PlotPose-response>))
  (cl:+ 0
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <PlotPose-response>))
  "Converts a ROS message object to a list"
  (cl:list 'PlotPose-response
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'PlotPose)))
  'PlotPose-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'PlotPose)))
  'PlotPose-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'PlotPose)))
  "Returns string type for a service object of type '<PlotPose>"
  "plot_tool/PlotPose")