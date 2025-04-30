package com.arkueid.app.ui.expression

import com.arkueid.app.data.ModelInfo
import com.arkueid.app.utils.MMKVUtil

data class Expression(
    private val modelInfo: ModelInfo,
    val name: String,
    private var _active: Boolean
) {

    var active: Boolean
        get() = _active
        set(value) {
            val expName = name
            if (value) {
                modelInfo.run {
                    model.addExpression(expName)
                    preference.run {
                        activeExpressions.run { if (!contains(expName)) add(expName) }

                        colorSchemes[expName]?.forEach { targetColor ->
                            when (targetColor.type) {
                                0 -> targetColor.color.run {
                                    model.setPartMultiplyColor(
                                        targetColor.targetIndex,
                                        r,
                                        g,
                                        b
                                    )
                                }

                                1 -> targetColor.color.run {
                                    model.setDrawableMultiColor(
                                        targetColor.targetIndex,
                                        r,
                                        g,
                                        b
                                    )
                                }
                            }
                        }
                    }
                }
            } else {
                modelInfo.run {
                    model.removeExpression(expName)

                    preference.run {
                        activeExpressions.remove(expName)

                        colorSchemes[expName]?.forEach { targetColor ->
                            when (targetColor.type) {
                                0 -> model.setPartMultiplyColor(
                                    targetColor.targetIndex,
                                    1.0f,
                                    1.0f,
                                    1.0f
                                )

                                1 -> model.setDrawableMultiColor(
                                    targetColor.targetIndex,
                                    1.0f,
                                    1.0f,
                                    1.0f
                                )
                            }
                        }
                    }
                }
            }
            MMKVUtil.schedule { mmkv ->
                mmkv.encode(modelInfo.name, modelInfo.preference)
            }
            _active = value
        }
}
