package com.example.player;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MyPlayer implements SurfaceHolder.Callback {
    private static final String TAG = "MyPlayer";

    static {
        System.loadLibrary("native-lib");
    }

    private String dataSource;

    private SurfaceHolder holder;

    private PrepareListener prepareListener;

    public void setDataSource(String url) {
        this.dataSource = url;
    }

    public void setPrepareListener(PrepareListener prepareListener) {
        this.prepareListener = prepareListener;
    }

    public void onError(int errorCode) {
        Log.e(TAG, "onError: " + errorCode);
    }

    /**
     * 准备，获取到视频的格式信息，编码格式等
     */
    public void prepare() {
        nativePrepare(dataSource);
    }

    public void onPrePare() {
        if (prepareListener != null) {
            prepareListener.onPrepare();
        }
    }

    public void start() {
        nativeStart();
    }

    public void stop() {

    }

    /**
     * 释放资源
     */
    public void release() {
        holder.removeCallback(this);
    }

    /**
     * 设置画布
     *
     * @param surfaceView 画布
     */
    public void setSurface(SurfaceView surfaceView) {
        holder = surfaceView.getHolder();
        holder.addCallback(this);
    }

    /**
     * 画布创建好
     *
     * @param holder
     */
    @Override
    public void surfaceCreated(SurfaceHolder holder) {

    }

    /**
     * 画布发生变化（横竖屏切换，home 键触发）
     *
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        nativeSetSurface(holder.getSurface());
    }

    /**
     * 画布被销毁（home键，退出当前界面）
     *
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }

    public interface PrepareListener {
        void onPrepare();
    }


    private native void nativePrepare(String dataSource);

    private native void nativeStart();

    private native void nativeSetSurface(Surface surface);
}
