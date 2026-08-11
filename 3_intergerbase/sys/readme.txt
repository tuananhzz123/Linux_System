# Chuyen sang root truoc khi thuc hien

root@beaglebone:/sys/class/gpio# ls
export  gpiochip512  gpiochip544  gpiochip576  gpiochip608  unexport
root@beaglebone:/sys/class/gpio# echo 638 > export 
root@beaglebone:/sys/class/gpio# ls
export  gpio638  gpiochip512  gpiochip544  gpiochip576  gpiochip608  unexport
root@beaglebone:/sys/class/gpio# cd gpio638
root@beaglebone:/sys/class/gpio/gpio638# echo out > direction 
root@beaglebone:/sys/class/gpio/gpio638# echo 1 > value
root@beaglebone:/sys/class/gpio/gpio638# echo 0 > value
root@beaglebone:/sys/class/gpio/gpio638# 


