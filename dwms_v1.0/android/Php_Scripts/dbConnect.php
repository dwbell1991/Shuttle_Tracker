<?php
    //Will eventually be placed onto a restful server
    define('HOST','dwbell_remote');
    define('USER','X');
    define('PASS','');
    define('DB','shuttles');

    $con = mysqli_connect(HOST,USER,PASS,DB) or die('Unable to Connect');