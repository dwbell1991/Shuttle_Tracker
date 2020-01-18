package com.dwbell.goldengeese.shuttlegpsapplication.shuttle;

public class BlueShuttle extends Shuttle {

    private final int hexBlue = 240;

   public BlueShuttle(String id, int utcTime, double lat, double lng, int conn, int satNum){
        super(id, utcTime, lat, lng, conn, satNum);
        this.hexMarkerColor = hexBlue;
        this.routeName = "Blue";

    }
}
