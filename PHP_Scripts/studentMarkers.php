<html>
<head>
        <title>Student Markers</title>
	<style>
	table {
		width: 600px;
	}
	td, tr {
		width:200px;
	}
	tr:nth-child(even) {
    		background-color: #dddddd;
	}

	</style>
</head>
<body>
<?php
ini_set('display_errors', true);

//Function to replicate mysql_result
function mysqli_result($result, $row, $field = 0) {
        $result->data_seek($row);
        $data = $result->fetch_array();
        return $data[$field];
}

//Database Connection
$conn = mysqli_connect("localhost","dwbell","daRealDrakeBell^8", "dwms_trunk");
if(mysqli_connect_errno()) {
        echo "Failed to connect to MySQL: " . mysqli_connect_error();
        exit();
}

$result = mysqli_query($conn,	"SElECT stud_ID, stud_fname, stud_lname, stud_lat, stud_lon 
				FROM Student
				INNER JOIN Student_Marker
				ON Student.stud_id = Student_Marker.mstud_ID;");
$row_count = mysqli_num_rows($result);
if($row_count > 0){
	$i = 0;
	echo "<h1>Student Markers</h1> <br />";
  	echo "<table>
                <tr>
                	<th>ID</th>
                	<th>Name</th>
                	<th>Lat,Long</th>
                </tr>";
    while ($i < $row_count) {
        $id = mysqli_result($result, $i, "stud_ID");
        $fname = mysqli_result($result, $i, "stud_fname");
        $lname = mysqli_result($result, $i, "stud_lname");
	$lat = mysqli_result($result, $i, "stud_lat");
	$lon = mysqli_result($result, $i, "stud_lon");
        $i++;
	echo"	
		<tr>
			<td>$id</td>
			<td>$fname $lname</td>
			<td>$lat, $lon</td>
		</tr>";
		
    	}
	echo "</table>";
mysqli_free_result($result);
}
else{
     echo "There are no student makers.";
}

mysqli_close($conn);

?>
<a href="http://dwms.zapto.org">Back to Home</a>
</body>
</html>
