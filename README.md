# log
log lib support multithread

## install
use **make** to create liblog.a   
you will find a liblog.a in **output/lib**  and some header file in **output/include**, they will be used in your program

## how to use it in your program
1. include the header file log.h in your source file

2. init log 
**init_log**: in the main thread   
**thread_init_log**: in the child thread
3. api
**log_fatal, log_warning, log_notice, log_trace, log_debug**
4. compile
when you compile, remember to add these compile args to tell the compiler where to search the lib and its header file, for example the demo in test dir  -L../output/lib -llog -I../output/ include 

## demo
> the test dir have a demo, it will tell you how to use it in your code and how to compile it with your program   
go to **test** dir, then execute **make**
