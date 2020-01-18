<html>
<head>
        <title>Insert Student</title>
</head>
<body>
<?php
ini_set('display_errors', true);

//Database Connection
$conn = mysqli_connect("localhost","dwbell","daRealDrakeBell^8", "dwms_trunk");
if(mysqli_connect_errno()) {
        exit();
}
$stud_ID = mysqli_real_escape_string($conn, $_REQUEST['stud_ID']);
$stud_fname = mysqli_real_escape_string($conn, $_REQUEST['stud_fname']);
$stud_lname = mysqli_real_escape_string($conn, $_REQUEST['stud_lname']);
$stud_phone_num = mysqli_real_escape_string($conn, $_REQUEST['stud_phone_num']);
$stud_email = mysqli_real_escape_string($conn, $_REQUEST['stud_email']);
$stud_passwd = mysqli_real_escape_string($conn, $_REQUEST['stud_passwd']);
$stud_schedule = mysqli_real_escape_string($conn, $_REQUEST['stud_schedule']);

$sql= "INSERT INTO Student (stud_ID, stud_fname, stud_lname, stud_phone_num, stud_email, stud_passwd, stud_schedule)
VALUES('$stud_ID','$stud_fname','$stud_lname','$stud_phone_num','$stud_email','$stud_passwd','$stud_schedule')";

if (!mysqli_query($conn, $sql)) {
	die('Error: ' . mysqli_error($conn));
}else {
	echo "1 record added</br>";
	echo "ID: $stud_ID</br>";
	echo "First Name: $stud_fname</br>";
	echo "Last Name: $stud_lname</br>";
	echo "Phone Num: $stud_phone_num</br>";
	echo "Schedules: $stud_schedule</br>";
}

mysqli_close($conn);

?>
<a href="http://dwms.zapto.org/getAllStudents.php">View Updated Student List</a><br/>
<a href="http://dwms.zapto.org/">Back to Home</a><br/>
</body>
</html>

