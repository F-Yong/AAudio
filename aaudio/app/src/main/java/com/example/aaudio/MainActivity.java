package com.example.aaudio;

import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;

import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.widget.Toast;
// 获取设备ID
import android.media.AudioManager;
import android.content.Context;
import android.media.AudioDeviceInfo;


public class MainActivity extends AppCompatActivity {
    final String TAG = "MainActivity";
    static {
        System.loadLibrary("aaudio_test");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });


        Button RecordButton = findViewById(R.id.Record);
        RecordButton.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View v) {
//                Toast.makeText(MainActivity.this, "recordButton", Toast.LENGTH_SHORT).show();'

                // 获取音频设备ID
                int device_ID = -1;
                AudioManager audioManager = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
                AudioDeviceInfo[] devices = audioManager.getDevices(AudioManager.GET_DEVICES_OUTPUTS);
                for (AudioDeviceInfo device : devices) {
                    if (device.getType() == AudioDeviceInfo.TYPE_BUILTIN_SPEAKER) {
                        // 找到ID
                        device_ID = device.getId();
                    }
                }
                Log.i(TAG,"device_ID: " + device_ID);

                int result = MyRecord(device_ID);
                Log.e(TAG,"Record: " + result);
            }
        });

        Button StopRecordButton = findViewById(R.id.StopRecord);
        StopRecordButton.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View v) {
//                Toast.makeText(MainActivity.this, "recordButton", Toast.LENGTH_SHORT).show();'

                int result = MyStopRecord();
                Log.e(TAG,"StopRecord: " + result);
            }
        });

        Button PlayButton = findViewById(R.id.Play);
        PlayButton.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View v) {
//                Toast.makeText(MainActivity.this, "playButton", Toast.LENGTH_SHORT).show();

                int result = MyPlay();
                Log.e(TAG, "Play: " + result);
            }
        });

        Button StopPlayButton = findViewById(R.id.StopPlay);
        StopPlayButton.setOnClickListener(new OnClickListener(){
            @Override
            public void onClick(View v) {
//                Toast.makeText(MainActivity.this, "recordButton", Toast.LENGTH_SHORT).show();'

                int result = MyStopPlay();
                Log.e(TAG,"StopPlay: " + result);
            }
        });

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

    private native int MyRecord(int device_ID);
    private native int MyStopRecord();
    private native int MyPlay();
    private native int MyStopPlay();
}




