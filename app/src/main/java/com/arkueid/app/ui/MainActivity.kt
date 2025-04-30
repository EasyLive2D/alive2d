package com.arkueid.app.ui

import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.os.Environment
import android.widget.SimpleAdapter
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.arkueid.app.R
import com.arkueid.app.databinding.ActivityMainBinding
import java.io.File


class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "MainActivity"
        private const val REQUEST_READ_EXTERNAL_STORAGE = 0;
    }

    private lateinit var modelListData: MutableList<Map<String, String>>

    private lateinit var binding: ActivityMainBinding
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)

        setContentView(binding.root)

        if (PackageManager.PERMISSION_GRANTED != checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)) {
            requestPermissions(
                arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE),
                REQUEST_READ_EXTERNAL_STORAGE
            );
        } else {
            loadModels()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == REQUEST_READ_EXTERNAL_STORAGE) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                loadModels()
            } else {
                Toast.makeText(this, "无法读取外部储存", Toast.LENGTH_LONG).show();
            }
        }
    }

    private fun loadModels() {
        val dir = File(Environment.getExternalStorageDirectory(), "A/Live2D")
        modelListData = mutableListOf()
        dir.list()?.forEach { subDirName ->
            val f = File(dir, subDirName)
            if (f.isDirectory) {
                val modelJsonName = f.list()?.firstOrNull { it.endsWith("model3.json") }
                val colorSchemePath = f.list()?.firstOrNull { it.endsWith("scheme.json") }
                if (modelJsonName != null) {
                    modelListData.add(
                        mapOf(
                            Pair("name", subDirName),
                            Pair("jsonPath", File(f, modelJsonName).absolutePath),
                            Pair("colorSchemePath", colorSchemePath?.let { File(f, colorSchemePath).absolutePath } ?: "" )
                        )
                    )
                }
            }
        }
        val adapter = SimpleAdapter(
            this, modelListData, R.layout.item_model,
            arrayOf("name", "jsonPath"), intArrayOf(R.id.name, R.id.jsonPath)
        )
        binding.modelList.adapter = adapter

        binding.modelList.setOnItemClickListener { _, _, position, _ ->
            val intent = Intent(this@MainActivity, Live2DActivity::class.java)
            intent.putExtra("name", modelListData[position]["name"])
            intent.putExtra("jsonPath", modelListData[position]["jsonPath"])
            intent.putExtra("colorSchemePath", modelListData[position]["colorSchemePath"])
            startActivity(intent)
        }
    }

}