package com.arkueid.alive2d;

import java.util.List;

/**
 * Wrapper for native class LAppModel
 */
public final class LAppModel {
    // native address/ptr for cpp object
    private final long nativeHandle;

    // create native object
    private static native long create();

    // release native object
    private static native void destroy(long handle);

    private LAppModel() {
        nativeHandle = create();
    }

    public static LAppModel obtain() {
        return new LAppModel();
    }

    protected void finalize() {
    }

    public void recycle() {
        destroy(nativeHandle);
    }

    private static native void loadModelJson(long handle, String fileName);

    public void loadModelJson(String fileName) {
        loadModelJson(nativeHandle, fileName);
    }

    private static native void resize(long handle, int ww, int wh);

    public void resize(int ww, int wh) {
        resize(nativeHandle, ww, wh);
    }

    private static native void draw(long handle);

    public void draw() {
        draw(nativeHandle);
    }

    private static native void startMotion(long handle, String group, int no, int priority, OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback);

    public void startMotion(String group, int no, int priority, OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback) {
        startMotion(nativeHandle, group, no, priority, onStartMotionCallback, onFinishMotionCallback);
    }

    public void startMotion(String group, int no, int priority, OnStartMotionCallback onStartMotionCallback) {
        startMotion(group, no, priority, onStartMotionCallback, null);
    }

    public void startMotion(String group, int no, int priority, OnFinishMotionCallback onFinishMotionCallback) {
        startMotion(group, no, priority, null, onFinishMotionCallback);
    }

    public void startMotion(String group, int no, int priority) {
        startMotion(group, no, priority, null, null);
    }

    private static native void startRandomMotion(long handle, String group, int priority, OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback);

    public void startRandomMotion(String group, int priority, OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback) {
        startRandomMotion(nativeHandle, group, priority, onStartMotionCallback, onFinishMotionCallback);
    }

    public void startRandomMotion(int priority, OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback) {
        startRandomMotion(null, priority, onStartMotionCallback, onFinishMotionCallback);
    }

    public void startRandomMotion(OnStartMotionCallback onStartMotionCallback, OnFinishMotionCallback onFinishMotionCallback) {
        startRandomMotion(3, onStartMotionCallback, onFinishMotionCallback);
    }

    public void startRandomMotion(OnFinishMotionCallback onFinishMotionCallback) {
        startRandomMotion(null, onFinishMotionCallback);
    }

    public void startRandomMotion(OnStartMotionCallback onStartMotionCallback) {
        startRandomMotion(onStartMotionCallback, null);
    }

    public void startRandomMotion() {
        startRandomMotion(null, null);
    }

    private static native void setExpression(long handle, String expressionId);

    public void setExpression(String expressionId) {
        setExpression(nativeHandle, expressionId);
    }

    private static native void setRandomExpression(long handle);

    public void setRandomExpression() {
        setRandomExpression(nativeHandle);
    }

    private static native boolean hitTest(long handle, String hitAreaName, float x, float y);

    public boolean hitTest(String hitAreaName, float x, float y) {
        return hitTest(nativeHandle, hitAreaName, x, y);
    }

    private static native boolean hasMocConsistencyFromFile(long handle, String mocFile);

    public boolean hasMocConsistencyFromFile(String mocFile) {
        return hasMocConsistencyFromFile(nativeHandle, mocFile);
    }

    private static native void drag(long handle, float x, float y);

    public void drag(float x, float y) {
        drag(nativeHandle, x, y);
    }

    private static native boolean isMotionFinished(long handle);

    public boolean isMotionFinished() {
        return isMotionFinished(nativeHandle);
    }

    private static native void setOffset(long handle, float dx, float dy);

    public void setOffset(float dx, float dy) {
        setOffset(nativeHandle, dx, dy);
    }

    private static native void setScale(long handle, float scale);

    public void setScale(float scale) {
        setScale(nativeHandle, scale);
    }

    private static native void rotate(long handle, float degrees);

    public void rotate(float degrees) {
        rotate(nativeHandle, degrees);
    }

    private static native void setParameterValue(long handle, String paramId, float value, float weight);

    public void setParameterValue(String paramId, float value, float weight) {
        setParameterValue(nativeHandle, paramId, value, weight);
    }

    public void setParameterValue(long handle, String paramId, float value) {
        setParameterValue(nativeHandle, paramId, value, 1.0f);
    }

    private static native void addParameterValue(long handle, String paramId, float value);

    public void addParameterValue(String paramId, float value) {
        addParameterValue(nativeHandle, paramId, value);
    }

    private static native void update(long handle);

    public void update() {
        update(nativeHandle);
    }

    private static native void setAutoBreathEnable(long handle, boolean enable);

    public void setAutoBreathEnable(boolean enable) {
        setAutoBreathEnable(nativeHandle, enable);
    }

    private static native void setAutoBlinkEnable(long handle, boolean enable);

    public void setAutoBlinkEnable(boolean enable) {
        setAutoBlinkEnable(nativeHandle, enable);
    }

    private static native int getParameterCount(long handle);

    public int getParameterCount() {
        return getParameterCount(nativeHandle);
    }

    private static native Parameter getParameter(long handle, int index);

    public Parameter getParameter(int index) {
        return getParameter(nativeHandle, index);
    }

    private static native float getParameterValue(long handle, int index);

    public float getParameterValue(int index) {
        return getParameterValue(nativeHandle, index);
    }

    private static native int getPartCount(long handle);

    public int getPartCount() {
        return getPartCount(nativeHandle);
    }

    private static native String getPartId(long handle, int index);

    public String getPartId(int index) {
        return getPartId(nativeHandle, index);
    }

    private static native String[] getPartIds(long handle);

    public String[] getPartIds() {
        return getPartIds(nativeHandle);
    }

    private static native void setPartOpacity(long handle, int index, float opacity);

    public void setPartOpacity(int index, float opacity) {
        setPartOpacity(nativeHandle, index, opacity);
    }

    private static native List<String> hitPart(long handle, float x, float y, boolean topOnly);

    public List<String> hitPart(float x, float y, boolean topOnly) {
        return hitPart(nativeHandle, x, y, topOnly);
    }

    public List<String> hitPart(float x, float y) {
        return hitPart(x, y, false);
    }

    private static native void setPartScreenColor(long handle, int index, float r, float g, float b, float a);

    public void setPartScreenColor(int index, float r, float g, float b, float a) {
        setPartScreenColor(nativeHandle, index, r, g, b, a);
    }

    private static native float[] getPartScreenColor(long handle, int index);

    public float[] getPartScreenColor(int index) {
        return getPartScreenColor(nativeHandle, index);
    }

    private static native void setPartMultiplyColor(long handle, int index, float r, float g, float b, float a);

    public void setPartMultiplyColor(int index, float r, float g, float b, float a) {
        setPartMultiplyColor(nativeHandle, index, r, g, b, a);
    }

    private static native float[] getPartMultiplyColor(long handle, int index);

    public float[] getPartMultiplyColor(int index) {
        return getPartMultiplyColor(nativeHandle, index);
    }

    private static native void clearMotions(long handle);

    public void clearMotions() {
        clearMotions(nativeHandle);
    }

    private static native void resetExpression(long handle);

    public void resetExpression() {
        resetExpression(nativeHandle);
    }

}
