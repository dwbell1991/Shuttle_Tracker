Overview of Phase III
By: Drake Bell and BLake Ross

To access our remote database please visit...
http://dwms.zapto.org

[Home Page]
This page displays a basic unorderd list with hyperlinks to each of the seven different
MySQL commands required for this project. It acts as the basic control mechanism to allow
the user to navigate throughout the website. Each of the seven queries will be explained 
below. 

[Get All Student] 
http://dwms.zapto.org/getAllStudents.php
This page presents all of the students in our database in a neatly laid out table
format. It uses the ORDER BY clause to list students names in alphabetical order for
easy reading. This query will be useful simply for keeping a running tally on the 
new users that may potentially use our product. 

[Insert a Student]
http://dwms.zapto.org/insertStudent.html
http://dwms.zapto.org/insert.php
This query first hits a form page from which the user can insert all information that 
they need to in order to add a student to the Student table. Student ID, First Name, 
Last Name, Phone Number, Email, Password, and Schedule. Once submitted, it will then 
run the PHP script insert.php and display a success message showing the values that 
were added succesfully. Fields that are not required, do not have to be entered in this
form. Naturally this query is useful because we need a means of adding users. There is
a link to check the updated student list from getAllStudents.

[Delete a Student] 
http://dwms.zapto.org/deleteStudent.html
http://dwms.zapto.org/delete.php
This query first hits a form page from which the user can enter a student id in 
order to process a delete. Once submitted it will hit the delete.php script and 
remove the specified user, and display a sucess message. This query is needed
because the user's will need to have the right to remove there account if they 
so choose. There is a link to check the updated student list from getAllStudents.

[Pull Student Markers]
http://dwms.zapto.org/studentMarkers.php
This php script will directly execute the query needed to display all student's
along with there associated markers. This script will be extremely useful when it
comes time to render multiple markers on a map, and can easily be translated to 
driver markers as well. 

[Get All Active Drivers]
http://dwms.zapto.org/getActiveDrivers.php
This PHP script will pull all active drivers currently in operation on a shuttle
route. It is formatted nicely in a table output. It will be useful in the future
because we will need to know which driver is currently operating so we can change
our device accordingly. 

[Get All Device Markers]
http://dwms.zapto.org/deviceMarkers.php
This PHP script pulls all of our current devices, there lat/long values, number of 
satellites connected to, and the speed of the current device in MPH. This script will 
be useful not only because of the lat/long coordinates but for diagnosing signal 
strength, and current shuttle speeds for outside algorithms. 

[Update a Student Marker]
http://dwms.zapto.org/updateStudentMarker.html
http://dwms.zapto.org/updateStudentMarker.php
This query first hits a form in which the user can enter a marker id which 
is currently alread in the table. They then can enter a new marker id as well 
as the latitude and longitude that our users and devices will need to update. 
On submit, it will update not only those values, but also by using MySQL's 
NOW() function on our data type 'timestamp' we can record live updates down 
to the second. 
