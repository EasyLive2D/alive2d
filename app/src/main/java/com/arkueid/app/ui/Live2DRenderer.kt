package com.arkueid.app.ui

import android.opengl.GLSurfaceView.Renderer
import android.util.Log
import com.arkueid.alive2d.Live2D
import com.arkueid.alive2d.Live2DModel
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class Live2DRenderer(private val model: Live2DModel): Renderer {
    private var lastCt: Long = 0L

    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        model.createRenderer(2)
        lastCt = System.currentTimeMillis()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        model.resize(width, height)
    }

    override fun onDrawFrame(gl: GL10?) {
        Live2D.clearBuffer()

        updateModel(model)
        model.draw()
    }

    private fun updateModel(model: Live2DModel) {
        val ct = System.currentTimeMillis()
        val delta = (ct - lastCt) / 1000f
        lastCt = ct

//        Log.d("TAG", "updateModel: $delta")

        var updated = false
        model.loadParameters()
        if (!model.isMotionFinished) {
            updated = model.updateMotion(delta)
        }
        model.saveParameters()
        if (!updated) {
//        Log.d("TAG", "updateModel: $delta")

            model.updateBlink(delta)
        }
        model.updateBreath(delta)
        model.updateDrag(delta)
        model.updateExpression(delta)
        model.updatePhysics(delta)
        model.updatePose(delta)
    }
}