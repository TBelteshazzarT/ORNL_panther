
(cl:in-package :asdf)

(defsystem "sdr_control-srv"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "SwitchVelocity" :depends-on ("_package_SwitchVelocity"))
    (:file "_package_SwitchVelocity" :depends-on ("_package"))
  ))