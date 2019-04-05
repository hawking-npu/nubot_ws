;; Auto-generated. Do not edit!


(when (boundp 'nubot_control::DebugInfo)
  (if (not (find-package "NUBOT_CONTROL"))
    (make-package "NUBOT_CONTROL"))
  (shadow 'DebugInfo (find-package "NUBOT_CONTROL")))
(unless (find-package "NUBOT_CONTROL::DEBUGINFO")
  (make-package "NUBOT_CONTROL::DEBUGINFO"))

(in-package "ROS")
;;//! \htmlinclude DebugInfo.msg.html


(defclass nubot_control::DebugInfo
  :super ros::object
  :slots (_d1 _d2 ))

(defmethod nubot_control::DebugInfo
  (:init
   (&key
    ((:d1 __d1) 0.0)
    ((:d2 __d2) 0.0)
    )
   (send-super :init)
   (setq _d1 (float __d1))
   (setq _d2 (float __d2))
   self)
  (:d1
   (&optional __d1)
   (if __d1 (setq _d1 __d1)) _d1)
  (:d2
   (&optional __d2)
   (if __d2 (setq _d2 __d2)) _d2)
  (:serialization-length
   ()
   (+
    ;; float32 _d1
    4
    ;; float32 _d2
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32 _d1
       (sys::poke _d1 (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _d2
       (sys::poke _d2 (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32 _d1
     (setq _d1 (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _d2
     (setq _d2 (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get nubot_control::DebugInfo :md5sum-) "1c78ad82c247783d0c3aa95ce147b845")
(setf (get nubot_control::DebugInfo :datatype-) "nubot_control/DebugInfo")
(setf (get nubot_control::DebugInfo :definition-)
      "float32 d1
float32 d2

")



(provide :nubot_control/DebugInfo "1c78ad82c247783d0c3aa95ce147b845")


