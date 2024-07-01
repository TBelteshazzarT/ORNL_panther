; Auto-generated. Do not edit!


(cl:in-package sdr_control-srv)


;//! \htmlinclude SwitchVelocity-request.msg.html

(cl:defclass <SwitchVelocity-request> (roslisp-msg-protocol:ros-message)
  ((switch_to
    :reader switch_to
    :initarg :switch_to
    :type cl:string
    :initform ""))
)

(cl:defclass SwitchVelocity-request (<SwitchVelocity-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SwitchVelocity-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SwitchVelocity-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name sdr_control-srv:<SwitchVelocity-request> is deprecated: use sdr_control-srv:SwitchVelocity-request instead.")))

(cl:ensure-generic-function 'switch_to-val :lambda-list '(m))
(cl:defmethod switch_to-val ((m <SwitchVelocity-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader sdr_control-srv:switch_to-val is deprecated.  Use sdr_control-srv:switch_to instead.")
  (switch_to m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SwitchVelocity-request>) ostream)
  "Serializes a message object of type '<SwitchVelocity-request>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'switch_to))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'switch_to))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SwitchVelocity-request>) istream)
  "Deserializes a message object of type '<SwitchVelocity-request>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'switch_to) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'switch_to) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SwitchVelocity-request>)))
  "Returns string type for a service object of type '<SwitchVelocity-request>"
  "sdr_control/SwitchVelocityRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SwitchVelocity-request)))
  "Returns string type for a service object of type 'SwitchVelocity-request"
  "sdr_control/SwitchVelocityRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SwitchVelocity-request>)))
  "Returns md5sum for a message object of type '<SwitchVelocity-request>"
  "d1597f0c930b70b3b5b1b67db31b5f98")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SwitchVelocity-request)))
  "Returns md5sum for a message object of type 'SwitchVelocity-request"
  "d1597f0c930b70b3b5b1b67db31b5f98")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SwitchVelocity-request>)))
  "Returns full string definition for message of type '<SwitchVelocity-request>"
  (cl:format cl:nil "string switch_to~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SwitchVelocity-request)))
  "Returns full string definition for message of type 'SwitchVelocity-request"
  (cl:format cl:nil "string switch_to~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SwitchVelocity-request>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'switch_to))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SwitchVelocity-request>))
  "Converts a ROS message object to a list"
  (cl:list 'SwitchVelocity-request
    (cl:cons ':switch_to (switch_to msg))
))
;//! \htmlinclude SwitchVelocity-response.msg.html

(cl:defclass <SwitchVelocity-response> (roslisp-msg-protocol:ros-message)
  ((switched
    :reader switched
    :initarg :switched
    :type cl:boolean
    :initform cl:nil))
)

(cl:defclass SwitchVelocity-response (<SwitchVelocity-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <SwitchVelocity-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'SwitchVelocity-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name sdr_control-srv:<SwitchVelocity-response> is deprecated: use sdr_control-srv:SwitchVelocity-response instead.")))

(cl:ensure-generic-function 'switched-val :lambda-list '(m))
(cl:defmethod switched-val ((m <SwitchVelocity-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader sdr_control-srv:switched-val is deprecated.  Use sdr_control-srv:switched instead.")
  (switched m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <SwitchVelocity-response>) ostream)
  "Serializes a message object of type '<SwitchVelocity-response>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:if (cl:slot-value msg 'switched) 1 0)) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <SwitchVelocity-response>) istream)
  "Deserializes a message object of type '<SwitchVelocity-response>"
    (cl:setf (cl:slot-value msg 'switched) (cl:not (cl:zerop (cl:read-byte istream))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<SwitchVelocity-response>)))
  "Returns string type for a service object of type '<SwitchVelocity-response>"
  "sdr_control/SwitchVelocityResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SwitchVelocity-response)))
  "Returns string type for a service object of type 'SwitchVelocity-response"
  "sdr_control/SwitchVelocityResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<SwitchVelocity-response>)))
  "Returns md5sum for a message object of type '<SwitchVelocity-response>"
  "d1597f0c930b70b3b5b1b67db31b5f98")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'SwitchVelocity-response)))
  "Returns md5sum for a message object of type 'SwitchVelocity-response"
  "d1597f0c930b70b3b5b1b67db31b5f98")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<SwitchVelocity-response>)))
  "Returns full string definition for message of type '<SwitchVelocity-response>"
  (cl:format cl:nil "bool switched~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'SwitchVelocity-response)))
  "Returns full string definition for message of type 'SwitchVelocity-response"
  (cl:format cl:nil "bool switched~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <SwitchVelocity-response>))
  (cl:+ 0
     1
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <SwitchVelocity-response>))
  "Converts a ROS message object to a list"
  (cl:list 'SwitchVelocity-response
    (cl:cons ':switched (switched msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'SwitchVelocity)))
  'SwitchVelocity-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'SwitchVelocity)))
  'SwitchVelocity-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'SwitchVelocity)))
  "Returns string type for a service object of type '<SwitchVelocity>"
  "sdr_control/SwitchVelocity")