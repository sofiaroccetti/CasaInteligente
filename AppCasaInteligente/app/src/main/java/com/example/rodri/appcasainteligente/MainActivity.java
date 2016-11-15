package com.example.rodri.appcasainteligente;


import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.AsyncTask;
import android.os.Bundle;
import android.speech.RecognizerIntent;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements OnClickListener, SensorEventListener {

    String comandoVoz = "";
    long last_update = 0, last_movement = 0;
    float prevX = 0, prevY = 0, prevZ = 0;
    float curX = 0, curY = 0, curZ = 0;
    int estadoVentana = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ImageButton imgMicrofono = (ImageButton) findViewById(R.id.imgMicrofono);

        Button btnEncenderCocina = (Button) findViewById(R.id.btnEncenderCocina);
        Button btnEncenderLiving = (Button) findViewById(R.id.btnEncenderLiving);
        Button btnEncenderDormitorio = (Button) findViewById(R.id.btnEncenderDormitorio);
        Button btnEncenderDescanso = (Button) findViewById(R.id.btnEncenderDescanso);
        Button btnApagarCocina = (Button) findViewById(R.id.btnApagarCocina);
        Button btnApagarLiving = (Button) findViewById(R.id.btnApagarLiving);
        Button btnApagarDormitorio = (Button) findViewById(R.id.btnApagarDormitorio);
        Button btnApagarDescanso = (Button) findViewById(R.id.btnApagarDescanso);

        solicitud("");

        btnEncenderCocina.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonCocinaoff");
            }
        });

        btnEncenderLiving.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonLivingoff");
            }
        });

        btnEncenderDormitorio.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonDormitoriooff");
            }
        });

        btnEncenderDescanso.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonDescansooff");
            }
        });

        btnApagarCocina.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonCocinaon");
            }
        });

        btnApagarLiving.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonLivingon");
            }
        });

        btnApagarDormitorio.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonDormitorioon");
            }
        });

        btnApagarDescanso.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                solicitud("?buttonDescansoon");
            }
        });

        imgMicrofono.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
                intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
                intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL, "en-US");
                intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, "es");
                intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_PREFERENCE, "es");
                intent.putExtra(RecognizerIntent.EXTRA_ONLY_RETURN_LANGUAGE_PREFERENCE, "es");
                intent.putExtra(RecognizerIntent.EXTRA_MAX_RESULTS, 5);

                try {
                    startActivityForResult(intent, 1);
                } catch (ActivityNotFoundException error) {
                    Toast.makeText(getApplicationContext(), "Reconocimiento de voz no soportado", Toast.LENGTH_LONG).show();
                }
            }


        });

    }

    public void onActivityResult(int requestCode, int resultCode, Intent datos) {
        TextView txtMicrofono = (TextView) findViewById(R.id.txtMicrofono);
        if(resultCode == Activity.RESULT_OK && datos != null) {
            ArrayList<String> text = datos.getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
            comandoVoz = text.get(0);
            txtMicrofono.setText(comandoVoz.toUpperCase());

            if(comandoVoz.toUpperCase().equals("ENCENDER COCINA")) {
                solicitud("?buttonCocinaoff");
            }

            if(comandoVoz.toUpperCase().equals("ENCENDER LIVING")) {
                solicitud("?buttonLivingoff");
            }

            if(comandoVoz.toUpperCase().equals("ENCENDER DORMITORIO")) {
                solicitud("?buttonDormitoriooff");
            }

            if(comandoVoz.toUpperCase().equals("ENCENDER DESCANSO")) {
                solicitud("?buttonDescansooff");
            }

            if(comandoVoz.toUpperCase().equals("APAGAR COCINA")) {
                solicitud("?buttonCocinaon");
            }

            if(comandoVoz.toUpperCase().equals("APAGAR LIVING")) {
                solicitud("?buttonLivingon");
            }

            if(comandoVoz.toUpperCase().equals("APAGAR DORMITORIO")) {
                solicitud("?buttonDormitorioon");
            }

            if(comandoVoz.toUpperCase().equals("APAGAR DESCANSO")) {
                solicitud("?buttonDescansoon");
            }
        }
    }

    public void solicitud(String comando) {

        ConnectivityManager connMgr = (ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);

        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();

        String url = "http://192.168.0.100/" + comando;

        if(networkInfo != null && networkInfo.isConnected()) {
            new DownloadWebpageTask().execute(url);
        } else {
            System.out.println("No hay conexión de red disponible");
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        synchronized (this) {
            long current_time = event.timestamp;

            curX = event.values[0];
            curY = event.values[1];
            curZ = event.values[2];

            if (prevX == 0 && prevY == 0 && prevZ == 0) {
                last_update = current_time;
                last_movement = current_time;
                prevX = curX;
                prevY = curY;
                prevZ = curZ;
            }

            long time_difference = current_time - last_update;
            if(time_difference > 0) {
                float movement = Math.abs((curX + curY + curZ) -
                        (prevX - prevY - prevZ)) / time_difference;
                int limit = 2000;
                float min_movement = 3E-6f;
                if (movement > min_movement) {
                    if (current_time - last_movement >= limit) {
                        Toast.makeText(getApplicationContext()
                                , "Hay movimiento de " + movement, Toast.LENGTH_SHORT).show();
                        if(estadoVentana == 0) {
                            solicitud("?abrirVentana");
                            estadoVentana = 1;
                        } else {
                            solicitud("?cerrarVentana");
                            estadoVentana = 0;
                        }

                    }
                    last_movement = current_time;
                }
                prevX = curX;
                prevY = curY;
                prevZ = curZ;
                last_update = current_time;
            }
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        SensorManager sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        List<Sensor> sensors = sensorManager.getSensorList(Sensor.TYPE_ACCELEROMETER);
        if(sensors.size() > 0) {
            sensorManager.registerListener(this, sensors.get(0), SensorManager.SENSOR_DELAY_GAME);
        }
    }

    @Override
    protected void  onStop() {
        super.onStop();
        SensorManager sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensorManager.unregisterListener(this);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    private class DownloadWebpageTask extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... urls) {

            Conectividad conectividad = new Conectividad();

            return conectividad.GetArduino(urls[0]);

        }
        @Override
        protected void onPostExecute(String result) {
            if(result != null) {
                System.out.println(result);
            } else {
                System.out.println("No network connection available.");
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onClick(View view) {

    }
}
