/************************************************************************
**      RollNo:2018201033          Name : Darshan Kansagara			   **
************************************************************************/

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        Mini-Bit-Torrent Project Guidlines
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

1.To compile client 
    --> make
 To clean solution
    --> make clean

2.To run Client 
    --> ./client <CLIENT_IP>:<UPLOAD_PORT> <TRACKER_IP_1>:<TRACKER_PORT_1> <TRACKER_IP_2>:<TRACKER_PORT_2> <log_file>
    Ex : ./client 192.168.1.20:8754 10.42.0.394:4444 10.42.0.666:5555 clientlog1


Client Functionality : 

    2.1 share any file
        --> share <local_file_path> <filename>.<file_extension>.mtorrent
        Ex : share /home/darshan/movie.mp4 moviefile.mp4.mtorrent

    2.2 Download/get command 
        --> get <path_to_.mtorrent_file> <destination_path>
        Ex : get moviefile.mp4.mtorrent newmovie.mp4

    2.3 show downloads
        --> show_downloads

    2.4 Remove share file and mtorrent
        --> remove <filename.mtorrent>
        Ex : moviefile.mp4.mtorrent
    
    2.5 close client
        --> close

Tracker/server side

3.To compile tracker 
    --> make
  To clean solution
    --> make clean
4.To run tracker
    -->./tracker <my_tracker_ip>:<my_tracker_port> <other_tracker_ip>:<other_tracker_port> <seederlist_file> <log_file>
    Ex : ./tracker 10.42.0.394:4444 10.42.0.666:5555 seederlist trackerlog

5. closed the tracker with Ctrl+ C

    
