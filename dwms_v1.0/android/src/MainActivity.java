package com.dwbell.goldengeese.shuttlegpsapplication;

import android.content.Intent;
import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Button blueRouteButton = (Button) findViewById(R.id.button2);
        blueRouteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goToMapsActivity(0);
            }
        });

        Button greenRouteButton = (Button) findViewById(R.id.button);
        greenRouteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goToMapsActivity(1);
            }
        });

        Button goldRouteButton = (Button) findViewById(R.id.button3);
        goldRouteButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                goToMapsActivity(2);
            }
        });

    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        int id = item.getItemId();

        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }


    private void goToMapsActivity(int routeParam) {
        int route = routeParam;
        System.out.printf("%d", route);
        Intent intent = new Intent(this, MapsActivity.class);
        intent.putExtra("Route Code",route);
        startActivity(intent);

    }

    //XXX Experiment with on actual device
    protected void onDestroy() {
        super.onDestroy();
        System.exit(0);
    }
}
