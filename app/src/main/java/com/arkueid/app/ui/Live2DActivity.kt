package com.arkueid.app.ui

import android.annotation.SuppressLint
import android.os.Bundle
import android.os.Environment
import android.view.MotionEvent
import android.view.View
import android.view.View.OnTouchListener
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.arkueid.alive2d.Live2D
import com.arkueid.alive2d.Live2DModel
import com.arkueid.app.R
import com.arkueid.app.data.Color
import com.arkueid.app.data.TargetColor
import com.arkueid.app.data.ModelInfo
import com.arkueid.app.data.ModelPreference
import com.arkueid.app.databinding.ActivityLive2DSceneBinding
import com.arkueid.app.utils.MMKVUtil
import com.google.android.material.tabs.TabLayoutMediator
import com.google.gson.JsonParser
import java.io.File
import java.io.FileReader

class Live2DActivity : AppCompatActivity(), OnTouchListener {

    private lateinit var binding: ActivityLive2DSceneBinding
    private lateinit var modelName: String
    private lateinit var modelJsonPath: String
    private lateinit var colorSchemePath: String
    private lateinit var modelInfo: ModelInfo

    private var lastDownX = 0.0f
    private var lastDownY = 0.0f
    private var moved = false
    private val moveThresholds = 25.0f

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityLive2DSceneBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        intent?.run {
            modelName = getStringExtra("name")!!
            modelJsonPath = getStringExtra("jsonPath")!!
            colorSchemePath = getStringExtra("colorSchemePath")!!
        }

        Live2D.init()

        initModel()

        binding.live2DSurfaceView.setLive2DModel(modelInfo.model)

        binding.live2DSurfaceView.setOnDragListener { _, event ->
            event?.let {
                binding.live2DSurfaceView.queueEvent {
                    modelInfo.model.drag(it.x, it.y)
                }
                true

            } ?: false
        }

        binding.live2DSurfaceView.setOnTouchListener(this)

        binding.recoverPoseBtn.setOnClickListener {
            binding.live2DSurfaceView.queueEvent {
                modelInfo.model.stopAllMotions()
                modelInfo.model.resetPose()
                modelInfo.model.resetAllParameters()
            }
        }

        binding.viewPager.adapter = PageAdapter(modelInfo, this)
        TabLayoutMediator(binding.tabLayout, binding.viewPager) { tab, index ->
            when (index) {
                0 -> tab.text = "表情"
            }
        }.attach()
    }

    private fun initModel() {
        var firstLoad = false
        val preference = MMKVUtil.mmkv.decodeParcelable(modelName, ModelPreference::class.java)
            ?: ModelPreference().apply {
                firstLoad = true
            }
        modelInfo = ModelInfo(modelName, preference)
        modelInfo.model = Live2DModel()
        modelInfo.model.loadModelJson(modelJsonPath)

        var dragIndex = 0
        var touchIndex = 0
        with(File(Environment.getExternalStorageDirectory(), "A/Live2D/public_motions")) {
            if (exists()) {
                val files = listFiles()
                files?.forEach { file ->
                    if (file.isFile && file.name.endsWith("motion3.json")) {
                        if (file.name.startsWith("touch")) {
                            modelInfo.model.loadExtraMotion(
                                "touch",
                                touchIndex++,
                                file.absolutePath
                            )
                        } else if (file.name.startsWith("drag")) {
                            modelInfo.model.loadExtraMotion("drag", dragIndex++, file.absolutePath)
                        }
                    }
                }
            }
        }


        if (firstLoad && colorSchemePath.isNotEmpty()) {
            with(FileReader(File(colorSchemePath))) {
                val json = JsonParser.parseReader(this).asJsonObject
                json.keySet().forEach { key ->
                    val targetColors = mutableListOf<TargetColor>()
                    json[key].asJsonArray.forEach { e ->
                        e.asJsonObject.let {
                            targetColors.add(
                                TargetColor(
                                    it["type"].asInt,
                                    it["targetIndex"].asInt,
                                    it["color"].asJsonArray.let { arr ->
                                        Color(arr[0].asFloat, arr[1].asFloat, arr[2].asFloat)
                                    })
                            )
                        }
                    }
                    modelInfo.preference.colorSchemes[key] = targetColors
                }
            }

        }

        preference.activeExpressions.forEach { name ->
            modelInfo.run {
                model.addExpression(name)

                preference.colorSchemes[name]?.forEach { targetColor ->
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
    }

    override fun onResume() {
        super.onResume()
        binding.live2DSurfaceView.onResume()
    }

    override fun onPause() {
        binding.live2DSurfaceView.onPause()
        super.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        modelInfo.model.destroy()
        Live2D.dispose()
    }

    @SuppressLint("ClickableViewAccessibility")
    override fun onTouch(v: View?, event: MotionEvent?): Boolean {
        return event?.let {
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    lastDownX = event.x
                    lastDownY = event.y
                    true
                }

                MotionEvent.ACTION_UP -> {
                    if (!moved) {
                        binding.live2DSurfaceView.queueEvent { modelInfo.model.startRandomMotion("touch") }
                    } else {
                        binding.live2DSurfaceView.queueEvent { modelInfo.model.startRandomMotion("drag") }
                    }
                    moved = false
                    binding.live2DSurfaceView.queueEvent {
                        modelInfo.model.drag(
                            binding.live2DSurfaceView.width / 2.0f,
                            binding.live2DSurfaceView.height / 2.0f
                        )
                    }
                    true
                }

                MotionEvent.ACTION_MOVE -> {
                    val dx = event.x - lastDownX
                    val dy = event.y - lastDownY
                    if (dx * dx + dy * dy > moveThresholds) {
                        moved = true
                        binding.live2DSurfaceView.queueEvent {
                            modelInfo.model.drag(event.x, event.y)
                        }
                    }
                    true
                }

                else -> false
            }
        } ?: false
    }
}