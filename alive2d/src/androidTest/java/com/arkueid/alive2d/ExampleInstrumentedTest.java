package com.arkueid.alive2d;

import android.content.Context;
import android.util.Log;

import androidx.test.platform.app.InstrumentationRegistry;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class ExampleInstrumentedTest {
    @Test
    public void testLive2D() {
        // Context of the app under test.
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();

        Live2D.setLogEnable(false);
        Live2D.init(appContext);
        Live2D.dispose();
        Live2D.clearBuffer(1.0f);
        Live2D.clearBuffer(1.0f, 0.5f);
        Live2D.clearBuffer(1.0f, 0.5f, 0.5f);
        Live2D.clearBuffer(1.0f, 0.5f, 0.5f, 0.5f);
        assertFalse(Live2D.logEnable());
    }

    @Test
    public void testLAppModel_create() throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Context appContext = InstrumentationRegistry.getInstrumentation().getTargetContext();
        Live2D.init(appContext);
        Class<LAppModel> clazz = LAppModel.class;
        Method method = clazz.getDeclaredMethod("create");
        method.setAccessible(true);
        Object result = method.invoke(null);
        assertNotEquals(result, null);
        long handle = (long)result;
        assertNotEquals(handle, 0);
        method = clazz.getDeclaredMethod("destroy", long.class);
        Log.d("", "" + handle);
        method.setAccessible(true);
        method.invoke(null, handle);
        Live2D.dispose();
    }
}