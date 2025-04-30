package com.arkueid.app.ui

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import com.arkueid.alive2d.Live2D
import com.arkueid.alive2d.Live2DModel


class Live2DSurfaceView(context: Context, attributeSet: AttributeSet? = null) :
    GLSurfaceView(context, attributeSet) {

    init {
        setEGLContextClientVersion(2)

    }

    private lateinit var _model: Live2DModel

    override fun onPause() {
        queueEvent {
            _model.destroyRenderer()
            Live2D.glRelease()
        }
        super.onPause()
    }

    fun setLive2DModel(model: Live2DModel) {
        _model = model
        setRenderer(Live2DRenderer(model))
    }

    fun setParameterValue(index: Int, value: Float) {
        _model.setAndSaveParameterValue(index, value)
    }
}