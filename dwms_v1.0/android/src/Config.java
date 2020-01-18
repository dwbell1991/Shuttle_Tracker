package com.dwbell.goldengeese.shuttlegpsapplication;

public class Config {
    //Address of our scripts of the CRUD
    public static final String URL_GET_SHUTTLE = "http://ec2-52-34-250-13.us-west-2.compute.amazonaws.com/getShuttle.php?id=";

    //JSON Tags
    public static final String TAG_JSON_ARRAY="result";
    public static final String TAG_ID = "id";
    public static final String TAG_LAT = "lat";
    public static final String TAG_LNG = "lng";
    public static final String TAG_HEAD = "head";
}