
(cl:in-package :asdf)

(defsystem "simulation-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils )
  :components ((:file "_package")
    (:file "feature" :depends-on ("_package_feature"))
    (:file "_package_feature" :depends-on ("_package"))
    (:file "uv" :depends-on ("_package_uv"))
    (:file "_package_uv" :depends-on ("_package"))
  ))