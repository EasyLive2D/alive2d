package com.arkueid.app.data

import com.arkueid.alive2d.Live2DModel

data class ModelInfo(
    val name: String,
    val preference: ModelPreference,
) {
    lateinit var model: Live2DModel
}
