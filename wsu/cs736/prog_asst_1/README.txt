Multithreaded Webserver.
Steps to run the program:
1. Making the executable: Use 'make' command to make the executable. The out file will be with the name mtserver.
2. Running the server: Use './mtserver' to run the server.
3. Open webbrowser using 'iceweasel'.
4. Type in address bar the address: http://localhost:50505/a1.html'. This displays Author 1 information: Jim Kurose.
5. The above step display Webpage 1 and it has link to another page a2.html. Click on the link. You can also type http://localhost:50505/a2.html in the address bar.
6. This displays Author 2 information: Kieth Ross.

Web browser displays the correct HTML pages.

About the application:
The application is a multithreaded webserver written in C language. It uses socket communication and opens socket in port 50505. It uses STREAM socket i.e., it uses TCP connection. It responds to HTTP GET messages with the requested file. Server is capable of only non-presistent HTTP messages i.e., for every object requested a new connection is established. If the requested file does not exist, it uses responds with NOT found message. If the requested file is found, it responds with the file and appropriate HTTP header.

Known issues:
The pages a1.html and a2. html should display the images of Kurose and Ross. But it does not display it. Only the html page is displayed. The wireshark capture indicates that HTTP response message is sent correctly but the image is corrupted and server sends all zeros for image. This issue is not yet fixed.
The server can respond to GET messages only.
