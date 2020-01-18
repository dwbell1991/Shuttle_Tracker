package com.dwbell.goldengeese.shuttlegpsapplication.shuttle;

import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;

public abstract class Shuttle {
    private String id;      //Bus ID
    private long utcTime;   //UTC time
    private double lat;     //Latitude
    private double lng;     //Longitude
    private int conn;       //Connection
    private int satNum;     //Satellites in views

    protected Marker marker;
    protected String routeName;
    protected int hexMarkerColor; //Respected marker color value;


    public Shuttle() {
        System.out.printf("No default shuttles allowed");
    }

    public Shuttle(String id, int utcTime, double lat, double lng, int conn, int satNum) {
        this.id = id;
        this.utcTime = utcTime;
        this.lat = lat;
        this.lng = lng;
        this.conn = conn;
        this.satNum = satNum;

    }

    public String getId() {
        return id;
    }

    public double getLat() {
        return lat;
    }

    public void setLat(double lat) {
        this.lat = lat;
    }

    public double getLng() {
        return lng;
    }

    public void setLng(double lng) {
        this.lng = lng;
    }

    public long getUTC() {
        return utcTime;
    }

    public void setUTC(long utcTime) {
        this.utcTime = utcTime;
    }

    public int getConn() {
        return conn;
    }

    public void setConn(int conn) {
        this.conn = conn;
    }

    public int getSatNum() {
        return satNum;
    }

    public void setSatNum(int satNum) {
        this.satNum = satNum;
    }

    public LatLng getLL() {
        LatLng temp = new LatLng(this.lat, this.lng);
        return temp;
    }

    public Marker getMarker() {
        return marker;
    }

    public void setMarker(Marker markerParam) {
        this.marker = markerParam;
    }

    public String getRouteName() {
        return routeName;
    }

    public void setRouteName(String routeName) {
        this.routeName = routeName;
    }

    public int getHexMarkerColor() {
        return hexMarkerColor;
    }
}