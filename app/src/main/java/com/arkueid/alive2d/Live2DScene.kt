package com.arkueid.alive2d

import android.annotation.SuppressLint
import android.opengl.GLSurfaceView.Renderer
import android.os.Bundle
import android.util.Log
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import androidx.lifecycle.lifecycleScope
import com.arkueid.alive2d.databinding.ActivityLive2DsceneBinding
import kotlinx.coroutines.launch
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class Live2DScene : AppCompatActivity() {

    lateinit var binding: ActivityLive2DsceneBinding
    lateinit var modelJsonPath: String
    lateinit var model: Live2DModel
    lateinit var renderer: Renderer
    var lastTime = 0L

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityLive2DsceneBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main)) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        getIntent()?.run {
            modelJsonPath = getStringExtra("jsonPath")!!
        }
    }

    @SuppressLint("SetTextI18n")
    private fun init() {
        model = Live2DModel()
        model.loadModelJson(modelJsonPath)

        val paramIds = model.parameterIds.joinToString { it }

        val partIds = model.partIds.joinToString { it }

        val drawableIds = model.drawableIds.joinToString { it }

        val motions = model.motions.map {
            it.key + ":\n" + it.value.joinToString("\n") { motion ->
                "File: ${motion["File"]}\n Sound: ${motion["Sound"]}"
            }
        }.joinToString("\n")

        binding.text.text =
            model.modelHomeDir +
                    "\n>>Param:\n" + paramIds +
                    "\n>>Part:\n" + partIds +
                    "\n>>Drawable:\n" + drawableIds +
                    "\n>>Motion:\n" + motions

        renderer = object : Renderer {
            override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
                model.createRenderer(2)
                lastTime = System.currentTimeMillis()
            }

            override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
                model.resize(width, height)
            }

            override fun onDrawFrame(gl: GL10?) {
                Live2D.clearBuffer()
                val ct = System.currentTimeMillis()
                val deltaTimeSecs = (ct - lastTime) / 1000.0f
                lastTime = ct

                model.update(deltaTimeSecs)
                model.draw()
            }

        }

        binding.glSurfaceView.run {
            setEGLContextClientVersion(2)
            setRenderer(renderer)
            setOnClickListener {
                model.startRandomMotion(null, MotionPriority.FORCE, { group, no ->
                    Log.d("MotionStart", "Motion Started: $group $no")
                }, { group, no ->
                    Log.d("MotionFinish", "Motion Finished: $group $no")
                })
            }
        }
    }

    override fun onStart() {
        super.onStart()
        Live2D.init()
        init()
    }

    override fun onResume() {
        super.onResume()
        binding.glSurfaceView.onResume()
    }

    override fun onPause() {
        super.onPause()
        binding.glSurfaceView.onPause()
    }

    override fun onStop() {
        super.onStop()
        model.destroy()
        Live2D.dispose()
    }
}