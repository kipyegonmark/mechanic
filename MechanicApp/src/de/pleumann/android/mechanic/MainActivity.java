package de.pleumann.android.mechanic;

import java.io.IOException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends Activity {

    GaugeView speed;
    GaugeView rpm;
    GaugeView load;
    GaugeView temp;
    GaugeView fuel;

    private volatile boolean alive;

    private String moduleName;
    
    private Runnable bluetoothRunnable = new Runnable() {
        public void run() {
            while (alive) {
                BluetoothHelper bt;
                
                try {
                    doSetTitle(getString(R.string.title_activity_main));
                    boolean showParams = true;
                    
                    bt = new BluetoothHelper(moduleName);
                    
                    try {
                        while (alive && bt.isConnected()) {
                            try {
                                String[] vals = bt.receive().split(",");
                                if (vals.length == 7) {
                                    if (showParams) {
                                        boolean slow = Boolean.parseBoolean(vals[0]);
                                        boolean extended = Boolean.parseBoolean(vals[1]);
                                    
                                        String params = ", " + (slow ? "250" : "500") + " kbps, " + (extended ? "extended" : "standard");
                                        doSetTitle(getString(R.string.title_activity_main) + " [" + moduleName + params + "]");                 
                                        showParams = false;
                                    }
                                    
                                    speed.setTarget(Float.parseFloat(vals[2]));
                                    rpm.setTarget(Float.parseFloat(vals[3]));
                                    load.setTarget(Float.parseFloat(vals[4]));
                                    temp.setTarget(Float.parseFloat(vals[5]));
                                    fuel.setTarget(Float.parseFloat(vals[6]));
                                }
                            } catch (Exception e) {
                                Log.e("XXX", "Error", e);
                                // Ignored
                            }
                        }
                    } finally {
                        bt.close();
                    }
                } catch (IOException ioe) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException ie) {
                        // Ignored
                    }
                }
            }
        }
    };

    private Runnable animationRunnable = new Runnable() {
        public void run() {
            while (alive) {
                speed.onStep();
                rpm.onStep();
                fuel.onStep();
                load.onStep();
                temp.onStep();
    
                try {
                    Thread.sleep(20);
                } catch (Exception e) {
                    // Ignored
                }
            }
        }
    };
    
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        speed = (GaugeView)findViewById(R.id.speedGauge);
        speed.setMin(0);
        speed.setMax(255);
        speed.setUnit(" km/h");
        speed.setTarget(0);
        
        rpm = (GaugeView)findViewById(R.id.rpmGauge);
        rpm.setMin(0);
        rpm.setMax(6000);
        rpm.setUnit(" rpm");
        rpm.setTarget(0);

        load = (GaugeView)findViewById(R.id.loadGauge);
        load.setMin(0);
        load.setMax(100);
        load.setUnit("% load");
        load.setTarget(0);

        temp = (GaugeView)findViewById(R.id.tempGauge);
        temp.setMin(-40);
        temp.setMax(215);
        temp.setUnit("¡C");
        temp.setTarget(-40);

        fuel = (GaugeView)findViewById(R.id.fuelGauge);
        fuel.setMin(0);
        fuel.setMax(100);
        fuel.setUnit("% fuel");
        fuel.setTarget(0);
        
        onMenuItemSelected(0, null);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
    @Override
    public boolean onMenuItemSelected(int featureId, MenuItem item) {
        final String[] modules = BluetoothHelper.getBondedDeviceNames();
        
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Select BT peer");
        builder.setItems(modules, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                moduleName = modules[which];
            }
        });
        builder.create().show();
        
        return true;
    }
    
    @Override
    protected void onResume() {
        super.onResume();
        
        alive = true;
        
        new Thread(bluetoothRunnable).start();
        new Thread(animationRunnable).start();
    }
    
    @Override
    protected void onPause() {
        super.onPause();
        
        alive = false;
    }
    
    private void doSetTitle(final String title) {
        runOnUiThread(new Runnable() {
            public void run() {
                setTitle(title);
            };
        });
        
    }
}
