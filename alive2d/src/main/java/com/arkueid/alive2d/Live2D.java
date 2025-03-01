package com.arkueid.alive2d;

import android.content.Context;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

public class Live2D {
    private static Context appContext;

    // Used to load the 'alive2d' library on application startup.
    static {
        System.loadLibrary("alive2d");
    }

    private static byte[] loadFile(String filePath) {
        try {
            if (filePath.startsWith("assets://")) {
                InputStream inputStream = appContext
                        .getAssets()
                        .open(filePath.replaceFirst("assets://", ""));
                int size = inputStream.available();
                byte[] bytes = new byte[size];
                inputStream.read(bytes);
                inputStream.close();
                return bytes;
            } else {
                File file = new File(filePath);
                FileInputStream inputStream = new FileInputStream(file);
                int size = (int) file.length();
                byte[] bytes = new byte[size];
                inputStream.read(bytes);
                inputStream.close();
                return bytes;
            }
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    // called before using live2d library
    private static native void init();
    public static void init(Context context) {
        appContext = context;
        init();
    }

    // called when no longer using live2d library
    public static native void dispose();

    // clear color buffer bit of current frame
    public static native void clearBuffer(float r, float g, float b, float a);

    public static void clearBuffer(float r, float g, float b) {
        clearBuffer(r, g, b, 0.0f);
    }

    public static void clearBuffer(float r, float g) {
        clearBuffer(r, g, 0.0f);
    }

    public static void clearBuffer(float r) {
        clearBuffer(r, 0.0f);
    }

    public static void clearBuffer() {
        clearBuffer(0.0f);
    }

    public static native void setLogEnable(boolean enable);

    public static native boolean logEnable();
}