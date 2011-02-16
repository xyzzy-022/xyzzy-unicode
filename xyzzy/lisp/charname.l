;;; -*- Mode: Lisp; Package: SYSTEM -*-
;;;
;;; This file is part of xyzzy.
;;;

(in-package "system")

(provide "charname")
(require "cnamedef")

(defun make-character-name-hash-table ()
  (let ((v (make-vector 65536))
	(h (make-hash-table :size 1 :test 'equal)))
    (dolist (x *character-name-alist*)
      (setf (gethash (car x) h) (code-char (cdr x))))
    h))

(setq *character-name-hash-table*
      (make-character-name-hash-table))
