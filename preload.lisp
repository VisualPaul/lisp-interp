(defvar defmacro
  (macro "defmacro" (name args &rest body)
     (cons 'defvar (cons name (cons (cons 'macro (cons (get-symbol-name name) (cons args body))) nil)))))
(defvar list
  (lambda "list" (&rest args)
    args))
(defmacro defun(name args &rest body)
  (cons 'defvar (cons name (cons (cons 'lambda (cons (get-symbol-name name) (cons args body))) nil))))

(defun list(&rest args)
  args)
(defun odd-p (arg)
  (= (mod arg 2) 1))
(defun even-p (arg)
  (= (mod arg 2) 0))
