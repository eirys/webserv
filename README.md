# webserv
My 42 project webserv, group project with @efrancon & @emtran

`make`

No leaks, or wild fds.
100% availability on siege, using epoll.

NB: 
- This code was adapted to Chrome.
- Note that, at the time we worked on the project, the subject was quite vague (webserv v.20.0): some rules were not clear about which C functions were allowed or not, and some requirements couldn't be properly met without the use of said 'forbidden' functions (e.g.: `stat`, `close`, `signal`...)
