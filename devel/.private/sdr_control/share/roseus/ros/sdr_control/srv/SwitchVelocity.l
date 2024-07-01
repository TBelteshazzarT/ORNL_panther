;; Auto-generated. Do not edit!


(when (boundp 'sdr_control::SwitchVelocity)
  (if (not (find-package "SDR_CONTROL"))
    (make-package "SDR_CONTROL"))
  (shadow 'SwitchVelocity (find-package "SDR_CONTROL")))
(unless (find-package "SDR_CONTROL::SWITCHVELOCITY")
  (make-package "SDR_CONTROL::SWITCHVELOCITY"))
(unless (find-package "SDR_CONTROL::SWITCHVELOCITYREQUEST")
  (make-package "SDR_CONTROL::SWITCHVELOCITYREQUEST"))
(unless (find-package "SDR_CONTROL::SWITCHVELOCITYRESPONSE")
  (make-package "SDR_CONTROL::SWITCHVELOCITYRESPONSE"))

(in-package "ROS")





(defclass sdr_control::SwitchVelocityRequest
  :super ros::object
  :slots (_switch_to ))

(defmethod sdr_control::SwitchVelocityRequest
  (:init
   (&key
    ((:switch_to __switch_to) "")
    )
   (send-super :init)
   (setq _switch_to (string __switch_to))
   self)
  (:switch_to
   (&optional __switch_to)
   (if __switch_to (setq _switch_to __switch_to)) _switch_to)
  (:serialization-length
   ()
   (+
    ;; string _switch_to
    4 (length _switch_to)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _switch_to
       (write-long (length _switch_to) s) (princ _switch_to s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _switch_to
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _switch_to (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;;
   self)
  )

(defclass sdr_control::SwitchVelocityResponse
  :super ros::object
  :slots (_switched ))

(defmethod sdr_control::SwitchVelocityResponse
  (:init
   (&key
    ((:switched __switched) nil)
    )
   (send-super :init)
   (setq _switched __switched)
   self)
  (:switched
   (&optional (__switched :null))
   (if (not (eq __switched :null)) (setq _switched __switched)) _switched)
  (:serialization-length
   ()
   (+
    ;; bool _switched
    1
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; bool _switched
       (if _switched (write-byte -1 s) (write-byte 0 s))
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; bool _switched
     (setq _switched (not (= 0 (sys::peek buf ptr- :char)))) (incf ptr- 1)
   ;;
   self)
  )

(defclass sdr_control::SwitchVelocity
  :super ros::object
  :slots ())

(setf (get sdr_control::SwitchVelocity :md5sum-) "d1597f0c930b70b3b5b1b67db31b5f98")
(setf (get sdr_control::SwitchVelocity :datatype-) "sdr_control/SwitchVelocity")
(setf (get sdr_control::SwitchVelocity :request) sdr_control::SwitchVelocityRequest)
(setf (get sdr_control::SwitchVelocity :response) sdr_control::SwitchVelocityResponse)

(defmethod sdr_control::SwitchVelocityRequest
  (:response () (instance sdr_control::SwitchVelocityResponse :init)))

(setf (get sdr_control::SwitchVelocityRequest :md5sum-) "d1597f0c930b70b3b5b1b67db31b5f98")
(setf (get sdr_control::SwitchVelocityRequest :datatype-) "sdr_control/SwitchVelocityRequest")
(setf (get sdr_control::SwitchVelocityRequest :definition-)
      "string switch_to
---
bool switched
")

(setf (get sdr_control::SwitchVelocityResponse :md5sum-) "d1597f0c930b70b3b5b1b67db31b5f98")
(setf (get sdr_control::SwitchVelocityResponse :datatype-) "sdr_control/SwitchVelocityResponse")
(setf (get sdr_control::SwitchVelocityResponse :definition-)
      "string switch_to
---
bool switched
")



(provide :sdr_control/SwitchVelocity "d1597f0c930b70b3b5b1b67db31b5f98")


