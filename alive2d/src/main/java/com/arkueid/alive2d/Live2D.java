package com.arkueid.alive2d;


public class Live2D {

    // Used to load the 'alive2d' library on application startup.
    static {
        System.loadLibrary("alive2d");
    }

    // called before using live2d library
    public static native void init();

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

    public static native void glRelease();
}