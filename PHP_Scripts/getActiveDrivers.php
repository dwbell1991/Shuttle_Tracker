<html>
<head>
        <title>Active Drivers</title>
        <style>
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

$result = mysqli_query($conn,   "SELECT dri_ID, dri_fname, dri_lname
				FROM Driver
				WHERE dri_ID 
				IN (SELECT dri_ID FROM Driver WHERE dri_is_active > 0);");
$row_count = mysqli_num_rows($result);
if($row_count > 0){
        $i = 0;
        echo "<h1>Active Drivers</h1> <br />";
        echo "<table>
                <tr>
                        <th>ID</th>
                        <th>Name</th>
                </tr>";
 while ($i < $row_count) {
        $id = mysqli_result($result, $i, "dri_ID");
        $fname = mysqli_result($result, $i, "dri_fname");
        $lname = mysqli_result($result, $i, "dri_lname");
        $i++;
        echo"
                <tr>
                        <td>$id</td>
                        <td>$fname $lname</td>
                </tr>";
        }
        echo "</table>";
mysqli_free_result($result);
}
else{
     echo "There are no drivers on shift";
}

mysqli_close($conn);

?>
<a href="http://dwms.zapto.org">Back to Home</a>
</body>
</html>
