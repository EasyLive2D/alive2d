package com.arkueid.alive2d

import android.content.Context
import android.opengl.GLSurfaceView
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class Live2DSurfaceView(context: Context) : GLSurfaceView(context) {
    override fun surfaceDestroyed(holder: SurfaceHolder) {
        super.surfaceDestroyed(holder)
    }
}

class Live2DRenderer : GLSurfaceView.Renderer {
    companion object {
        private const val TAG = "MainActivity"
    }

    lateinit var model: LAppModel
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        model = LAppModel.obtain().apply {
            loadModelJson("assets://Mao/Mao.model3.json")
        }
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        model.resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        gl?.glClearColor(0.0f, 0.0f, 0.0f, 0.0f)
        gl?.glClear(GL10.GL_COLOR_BUFFER_BIT)
        model.update()
        model.draw()
    }

}

class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "MainActivity"
    }

    lateinit var glSurfaceView: Live2DSurfaceView
    lateinit var renderer: Live2DRenderer
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        glSurfaceView = Live2DSurfaceView(this)
        glSurfaceView.setEGLContextClientVersion(2)
        renderer = Live2DRenderer()
        glSurfaceView.setRenderer(renderer)
        setContentView(glSurfaceView)

        glSurfaceView.setOnClickListener {
            it.post {
                renderer.model.startRandomMotion({ group, no ->
                    Log.e(TAG, "start motion: $group $no")
                }, {
                    Log.e(TAG, "finish motion")
                })
            }
        }
    }

    override fun onStart() {
        super.onStart()
        Live2D.init(applicationContext)
    }

    override fun onResume() {
        super.onResume()
        glSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        glSurfaceView.onPause()
        glSurfaceView.post { // 防止最后一帧还未画完就被释放
            renderer.model.recycle()
        }
    }

    override fun onStop() {
        super.onStop()
        Live2D.dispose()
    }

    override fun onDestroy() {
        super.onDestroy()
    }
}