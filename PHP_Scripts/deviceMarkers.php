<html>
<head>
        <title>Device Markers</title>
        <style>
        table {
                width: 800px;
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

$result = mysqli_query($conn,   "SELECT Distinct dev_ID, dev_lat, dev_lon, dev_sat_num, dev_speed
				FROM Device natural join Device_Marker
				WHERE dev_ID = mdev_ID;
");
$row_count = mysqli_num_rows($result);
if($row_count > 0){
        $i = 0;
        echo "<h1>Device Markers</h1> <br />";
        echo "<table>
                <tr>
                        <th>ID</th>
                        <th>Lat,Long</th>
			<th>Number of Satellites</th>
			<th>Speed (MPH)</th>
                </tr>";
 while ($i < $row_count) {
        $id = mysqli_result($result, $i, "dev_ID");
        $lat = mysqli_result($result, $i, "dev_lat");
        $lon = mysqli_result($result, $i, "dev_lon");
        $sat = mysqli_result($result, $i, "dev_sat_num");
        $speed = mysqli_result($result, $i, "dev_speed");
        $i++;
        echo"
                <tr>
                        <td>$id</td>
                        <td>$lat, $lon</td>
			<td>$sat</td>
			<td>$speed</td>
                </tr>";

        }
        echo "</table>";
mysqli_free_result($result);
}
else{
     echo "There are no devices with associated values";
}

mysqli_close($conn);

?>
<a href="http://dwms.zapto.org">Back to Home</a>
</body>
</html>
