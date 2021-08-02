# Abstract
* execute the next pipes asynchronously  

# Reason
* since the rea-python is realized by the eventloop of qt, it will limited by the call stack depths on the same thread. In some cases, such as simulated timer by self loopable calling the same pipe, the call stack depths is unlimited  
* sometimes we don't want to block the execution on the same thread  

# Reference
[pipe](../pipe.md)