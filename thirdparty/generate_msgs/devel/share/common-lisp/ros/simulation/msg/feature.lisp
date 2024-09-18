; Auto-generated. Do not edit!


(cl:in-package simulation-msg)


;//! \htmlinclude feature.msg.html

(cl:defclass <feature> (roslisp-msg-protocol:ros-message)
  ((stamp
    :reader stamp
    :initarg :stamp
    :type cl:real
    :initform 0)
   (data
    :reader data
    :initarg :data
    :type (cl:vector simulation-msg:uv)
   :initform (cl:make-array 0 :element-type 'simulation-msg:uv :initial-element (cl:make-instance 'simulation-msg:uv))))
)

(cl:defclass feature (<feature>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <feature>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'feature)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name simulation-msg:<feature> is deprecated: use simulation-msg:feature instead.")))

(cl:ensure-generic-function 'stamp-val :lambda-list '(m))
(cl:defmethod stamp-val ((m <feature>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation-msg:stamp-val is deprecated.  Use simulation-msg:stamp instead.")
  (stamp m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <feature>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader simulation-msg:data-val is deprecated.  Use simulation-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <feature>) ostream)
  "Serializes a message object of type '<feature>"
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'stamp)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'stamp) (cl:floor (cl:slot-value msg 'stamp)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (roslisp-msg-protocol:serialize ele ostream))
   (cl:slot-value msg 'data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <feature>) istream)
  "Deserializes a message object of type '<feature>"
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stamp) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:setf (cl:aref vals i) (cl:make-instance 'simulation-msg:uv))
  (roslisp-msg-protocol:deserialize (cl:aref vals i) istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<feature>)))
  "Returns string type for a message object of type '<feature>"
  "simulation/feature")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'feature)))
  "Returns string type for a message object of type 'feature"
  "simulation/feature")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<feature>)))
  "Returns md5sum for a message object of type '<feature>"
  "10db7163beffaf7a5f5e9feb3a765386")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'feature)))
  "Returns md5sum for a message object of type 'feature"
  "10db7163beffaf7a5f5e9feb3a765386")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<feature>)))
  "Returns full string definition for message of type '<feature>"
  (cl:format cl:nil "~%time stamp~%simulation/uv[] data~%~%================================================================================~%MSG: simulation/uv~%uint64 id~%float64 x~%float64 y~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'feature)))
  "Returns full string definition for message of type 'feature"
  (cl:format cl:nil "~%time stamp~%simulation/uv[] data~%~%================================================================================~%MSG: simulation/uv~%uint64 id~%float64 x~%float64 y~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <feature>))
  (cl:+ 0
     8
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ (roslisp-msg-protocol:serialization-length ele))))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <feature>))
  "Converts a ROS message object to a list"
  (cl:list 'feature
    (cl:cons ':stamp (stamp msg))
    (cl:cons ':data (data msg))
))
