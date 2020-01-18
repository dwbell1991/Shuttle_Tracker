<html>
<head>
        <title>Updated Student Marker</title>
</head>
<body>
<?php
ini_set('display_errors', true);

//Database Connection
$conn = mysqli_connect("localhost","dwbell","daRealDrakeBell^8", "dwms_trunk");
if(mysqli_connect_errno()) {
        echo "Failed to connect to MySQL: " . mysqli_connect_error();
        exit();
}

$oldID = mysqli_real_escape_string($conn, $_REQUEST['old_ID']);
$newID = mysqli_real_escape_string($conn, $_REQUEST['new_ID']);
$lat = mysqli_real_escape_string($conn, $_REQUEST['stud_lat']);
$lon = mysqli_real_escape_string($conn, $_REQUEST['stud_lon']);

$sql = "update Student_Marker 
	SET mstud_ID = $newID,
	stud_lat = $lat,
	stud_lon = $lon,
	stud_time = NOW(),
	stud_image = '/path/to/image.png'
 	WHERE mstud_ID = $oldID;";

if (!mysqli_query($conn, $sql)) {
        echo 'Error: Student Marker not found<br/>';
        echo '<a href="http://dwms.zapto.org">Back to Home</a>';
	echo "Failed to connect to MySQL: " . mysqli_connect_error();	
        die();
}else {
        echo 	"Student Marker ID: $oldID has been changed to $newID with the follow...<br/>;
		Latitude:  $lat<br/>
		Longitude: $lon<br/>
		Time: updated to current local time using NOW()<br/>
		Student Image: set to its default<br/>";
		
}

mysqli_close($conn);
?>
<a href="http://dwms.zapto.org/studentMarkers.php">View Updated Student Markers</a><br/>
<a href="http://dwms.zapto.org">Back to Home</a><br/>
</body>
</html>
