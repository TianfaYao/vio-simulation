;; Auto-generated. Do not edit!


(when (boundp 'simulation::feature)
  (if (not (find-package "SIMULATION"))
    (make-package "SIMULATION"))
  (shadow 'feature (find-package "SIMULATION")))
(unless (find-package "SIMULATION::FEATURE")
  (make-package "SIMULATION::FEATURE"))

(in-package "ROS")
;;//! \htmlinclude feature.msg.html


(defclass simulation::feature
  :super ros::object
  :slots (_stamp _data ))

(defmethod simulation::feature
  (:init
   (&key
    ((:stamp __stamp) (instance ros::time :init))
    ((:data __data) ())
    )
   (send-super :init)
   (setq _stamp __stamp)
   (setq _data __data)
   self)
  (:stamp
   (&optional __stamp)
   (if __stamp (setq _stamp __stamp)) _stamp)
  (:data
   (&rest __data)
   (if (keywordp (car __data))
       (send* _data __data)
     (progn
       (if __data (setq _data (car __data)))
       _data)))
  (:serialization-length
   ()
   (+
    ;; time _stamp
    8
    ;; simulation/uv[] _data
    (apply #'+ (send-all _data :serialization-length)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; time _stamp
       (write-long (send _stamp :sec) s) (write-long (send _stamp :nsec) s)
     ;; simulation/uv[] _data
     (write-long (length _data) s)
     (dolist (elem _data)
       (send elem :serialize s)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; time _stamp
     (send _stamp :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _stamp :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; simulation/uv[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (let (r) (dotimes (i n) (push (instance simulation::uv :init) r)) r))
     (dolist (elem- _data)
     (send elem- :deserialize buf ptr-) (incf ptr- (send elem- :serialization-length))
     ))
   ;;
   self)
  )

(setf (get simulation::feature :md5sum-) "10db7163beffaf7a5f5e9feb3a765386")
(setf (get simulation::feature :datatype-) "simulation/feature")
(setf (get simulation::feature :definition-)
      "
time stamp
simulation/uv[] data

================================================================================
MSG: simulation/uv
uint64 id
float64 x
float64 y
")



(provide :simulation/feature "10db7163beffaf7a5f5e9feb3a765386")


