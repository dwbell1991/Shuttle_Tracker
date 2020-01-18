package com.dwbell.goldengeese.shuttlegpsapplication.shuttle;

public class GoldShuttle extends Shuttle {

    private final int hexGold = 55;

    public GoldShuttle(String id, int utcTime, double lat, double lng, int conn, int satNum){
        super(id, utcTime, lat, lng, conn, satNum);
        this.hexMarkerColor = hexGold;
        this.routeName = "Gold";
    }
}