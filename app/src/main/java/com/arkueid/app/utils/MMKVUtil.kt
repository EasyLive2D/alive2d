package com.arkueid.app.utils

import android.os.Handler
import android.os.Looper
import com.tencent.mmkv.MMKV

object MMKVUtil {

    private val _mmkv by lazy { MMKV.defaultMMKV() }

    private val handler = Handler(Looper.getMainLooper())

    private var runnable: Runnable? = null

    fun schedule(action: (mmkv: MMKV) -> Unit) {
        runnable?.let { handler.removeCallbacks(it) }

        runnable = Runnable { action(_mmkv) }.apply {
            handler.postDelayed(this, 2000)
        }

    }

    val mmkv = _mmkv
}