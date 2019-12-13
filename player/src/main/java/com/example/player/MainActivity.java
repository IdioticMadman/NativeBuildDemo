package com.example.player;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    final String url = "http://tx2play1.douyucdn.cn/live/288016rlols5.flv";
//    final String url = "rtmp://58.200.131.2:1935/livetv/hunantv";

    private MyPlayer myPlayer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SurfaceView surfaceView = findViewById(R.id.surface);
        myPlayer = new MyPlayer();
        myPlayer.setDataSource(url);
        myPlayer.setSurface(surfaceView);
        myPlayer.setPrepareListener(new MyPlayer.PrepareListener() {
            @Override
            public void onPrepare() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, "视频准备就绪", Toast.LENGTH_SHORT)
                                .show();
                        myPlayer.start();
                    }
                });

            }
        });
    }

    public void start(View view) {
        myPlayer.prepare();
    }
}
