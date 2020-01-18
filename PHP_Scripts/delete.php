<html>
<head>
        <title>Deleted Student</title>
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

$stud_ID = mysqli_real_escape_string($conn, $_REQUEST['stud_ID']);

$sql = "DELETE FROM Student WHERE stud_ID=$stud_ID";

if (!mysqli_query($conn, $sql)) {
	echo 'Error: Student ID not found<br/>';
	echo '<a href="http://dwms.zapto.org">Back to Home</a>';
	die();
}else {
	echo "Student ID: $stud_ID has been deleted!<br/>";
}

mysqli_close($conn);
?>
<a href="http://dwms.zapto.org/getAllStudents.php">View Updated Students List</a><br/>
<a href="http://dwms.zapto.org">Back to Home</a><br/>
</body>
</html>
