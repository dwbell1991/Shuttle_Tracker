package com.dwbell.goldengeese.shuttlegpsapplication;

import android.graphics.Color;
import android.os.AsyncTask;
import android.support.v4.app.FragmentActivity;
import android.os.Bundle;

import com.dwbell.goldengeese.shuttlegpsapplication.shuttle.*;
import com.dwbell.goldengeese.shuttlegpsapplication.shuttle.Shuttle;
import com.google.android.gms.maps.CameraUpdateFactory;
import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.PolylineOptions;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class MapsActivity extends FragmentActivity implements OnMapReadyCallback {

    //Google Map Object
    private GoogleMap gMap;
    //Sentinel for work loop
    private boolean running;

    //Depicts which route was chosen (Blue, Green, Gold)
    private int routeCode;

    //Filenames for specific routes
    private final String[] busRouteNames = {"BlueRoute.txt", "GreenRoute.txt", "GoldRoute.txt"};
    //Line options for specified bus route
    private PolylineOptions busRoutePolyOptions;

    //Holds all shuttle lists
    HashMap<Integer, ArrayList<Shuttle>> map = new HashMap<>();

    //List of specific shuttle routes
    private List<BlueShuttle> blueList = new ArrayList<>();
    private List<GreenShuttle> greenList = new ArrayList<>();
    private List<GoldShuttle> goldList = new ArrayList<>();

    /***************************************************************
     * Name: onCreate
     * Desc: Called during map creation. Initializes running to true
     * and grabs the intent from MainActivity which describes which
     * route was chosen. This code is stored in routeCode.
     * Intent Codes
     * 0 - Blue Route
     * 1 - Green Route
     * 2 - Gold Route
     ***************************************************************/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_maps);
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        //Grabbing bus route code, 0 is a default
        routeCode = getIntent().getIntExtra("Route Code", 0);

        //Work loop to running
        running = true;

        //Put shuttle lists into a hashmap
        map.put(0, new ArrayList<Shuttle>(blueList));
        map.put(1, new ArrayList<Shuttle>(greenList));
        map.put(2, new ArrayList<Shuttle>(goldList));
    }

    /***************************************************************
     * Name: onMapReady
     * Desc: Called once the map is ready to worked with. Also initializes
     * some basic map features.
     ***************************************************************/
    @Override
    public void onMapReady(GoogleMap map) {
        this.gMap = map;
        initMap();          //Specify bus route polyline options
        initBusRoute();     //Load from file a specified bus route
        initShuttleList();  //Load up predefined shuttles
        update();           //Call upon update which will handle majority of work
    }

    /***************************************************************
     * Name: initMap
     * Desc: Changes google maps line options to respected route
     * color.
     ***************************************************************/
    public void initMap() {
        //Blue Route
        if (routeCode == 0) {
            busRoutePolyOptions = new PolylineOptions().color(Color.BLUE).width(10);
        }
        //Green Route
        else if (routeCode == 1) {
            busRoutePolyOptions = new PolylineOptions().color(Color.GREEN).width(10);
        }
        //Gold Route
        else if (routeCode == 2) {
            busRoutePolyOptions = new PolylineOptions().color(Color.YELLOW).width(10);
        }
    }

    /***************************************************************
     * Name: initBusRoute
     * Desc: Based upon what value was passed representing 'routeCode'
     * it will draw the according color polyline to create effect of a
     * drawn complete bus route (Entire route).
     * 0 - Blue Route
     * 1 - Green Route
     * 2 - Gold Route
     ***************************************************************/
    public void initBusRoute() {

        //Local holder, to draw map
        LatLng mapLL;

        //Preparing for buffered read through input stream.
        BufferedReader reader = null;
        String delims = ",";

        try {
            reader = new BufferedReader(new InputStreamReader(getAssets().open(busRouteNames[routeCode])));

            String mLine;
            //Read until EOF
            while ((mLine = reader.readLine()) != null) {
                String[] tokens = mLine.split(delims);
                double latX = Double.parseDouble(tokens[0]);
                double lngY = Double.parseDouble(tokens[1]);
                mapLL = new LatLng(latX, lngY);

                //Drawing the routes outline.
                gMap.addPolyline(busRoutePolyOptions.add(mapLL));
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    /***************************************************************
     * Name: initShuttleList
     * Desc: Initializes all appropriate shuttles, the only required
     * field is 'id' field which must represent the actual shuttle number
     * located on the side of an actual UNO shuttle bus. All other fields will
     * be updated when the server is called.
     * Format of any type of shuttle...
     * String id, int utcTime, double lat, double lng, String conn, int satNum
     ***************************************************************/
    public void initShuttleList() {

        //Shuttle #215: Blue Route
        BlueShuttle shuttle_215 = new BlueShuttle("215", 0, 0, 0, 0, 0);
        map.get(0).add(shuttle_215);

        //Shuttle #216: Green Route
        GreenShuttle shuttle_216 = new GreenShuttle("216", 0, 0, 0, 0, 0);
        map.get(1).add(shuttle_216);

        //Shuttle #217: Green Route
        GreenShuttle shuttle_217 = new GreenShuttle("217", 0, 0, 0, 0, 0);
        map.get(1).add(shuttle_217);
    }

    /***************************************************************
     * Name: update
     * Desc: This is the main working thread. It is called once
     * every 4 seconds to update the map GUI based on getShuttle()
     * into showShuttle() data.
     ***************************************************************/
    public void update() {
        //Create a new thread
        new Thread() {
            @Override
            public void run() {

                while (running) {
                    runOnUiThread(new Runnable() {

                        @Override
                        public void run() {

                            for (int i = 0; i < map.get(routeCode).size(); i++) {
                                getShuttle(map.get(routeCode).get(i).getId(), i);

                                if (map.get(routeCode).get(i).getLat() != 0 && map.get(routeCode).get(i).getLng() != 0) {
                                    System.out.printf("%f %f\n", map.get(routeCode).get(i).getLat(),map.get(routeCode).get(i).getLng());
                                    updateMarker(i);
                                    updateCamera();
                                }
                            }
                        }
                    });
                    try {
                        Thread.sleep(4000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        }.start();
    }

    /***************************************************************
     * Name: getShuttle
     * Desc: Using Async as a background thread to process map gui.
     * Uses the send request to contact the server by shuttle
     * id. It will also invoke the showShuttle method. idParam is
     * the id of specified shuttle.
     ***************************************************************/
    private void getShuttle(String idParam, int indexParam) {
        final String id = idParam;
        final int index = indexParam;

        class GetShuttle extends AsyncTask<Void, Void, String> {
            @Override
            protected void onPreExecute() {
                super.onPreExecute();
            }

            @Override
            protected void onPostExecute(String s) {
                super.onPostExecute(s);
                showShuttle(s, index);
            }

            @Override
            protected String doInBackground(Void... params) {
                RequestHandler rh = new RequestHandler();
                String s = rh.sendGetRequestParam(Config.URL_GET_SHUTTLE, id);
                return s;
            }
        }
        GetShuttle gs = new GetShuttle();
        gs.execute();
    }

    /***************************************************************
     * Name: showShuttle
     * Desc: Called from getShuttle, it takes returned parameters
     * which are in a JSON format and assigns the correct values to
     * allow updating.
     ***************************************************************/
    private void showShuttle(String json, int index) {

        try {
            JSONObject jsonObject = new JSONObject(json);
            JSONArray result = jsonObject.getJSONArray(Config.TAG_JSON_ARRAY);
            JSONObject c = result.getJSONObject(0);

            //Parsing out JSON data and storing it into respective shuttle list objects
            map.get(routeCode).get(index).setUTC(Long.parseLong(c.getString(Config.TAG_UTC)));
            map.get(routeCode).get(index).setLat(Double.parseDouble(c.getString(Config.TAG_LAT)));
            map.get(routeCode).get(index).setLng(Double.parseDouble(c.getString(Config.TAG_LNG)));
            map.get(routeCode).get(index).setConn(Integer.parseInt(c.getString(Config.TAG_CONN)));
            map.get(routeCode).get(index).setSatNum(Integer.parseInt(c.getString(Config.TAG_SATVIEW)));

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    /***************************************************************
     * Name: updateCamera
     * Desc: Moves the camera to hover over a specified point
     ***************************************************************/
    public void updateCamera() {

        //XXX Change in the future to follow human
        gMap.animateCamera(CameraUpdateFactory.newLatLngZoom(map.get(routeCode).get(0).getLL(), 16));
    }

    /***************************************************************
     * Name: updateMarker
     * Desc: Updates a marker based on "routeCode" and the parameter
     * index which specifies a specific index in a shuttle list.
     ***************************************************************/
    public void updateMarker(int index) {

        //If old marker exists, remove it
        if(map.get(routeCode).get(index).getMarker() != null){
            map.get(routeCode).get(index).getMarker().remove();
        }

        //add a new marker based on LatLng variable stored in "Shuttle" object
        map.get(routeCode).get(index).setMarker(gMap.addMarker(new MarkerOptions()
                        .position(map.get(routeCode).get(index).getLL())
                        .icon(BitmapDescriptorFactory.defaultMarker(map.get(routeCode).get(index).getHexMarkerColor())))
        );
    }


    /***************************************************************
     * Name: onPause
     * Desc: Android method overridden to stop the running loop and
     * set routeCode back to 0.
     ***************************************************************/
    @Override
    protected void onPause() {
        running = false;
        routeCode = 0;
        super.onPause();
    }

    /***************************************************************
     * Name: onPause
     * Desc: XXXXXXXXXX, this method may prevent when the screen
     * goes black to then start the loop running again. ?????????
     ***************************************************************/
    @Override
    protected void onResume(){
        running = true;
        super.onResume();
    }
}