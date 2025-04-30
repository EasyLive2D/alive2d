package com.arkueid.app.utils

import com.google.gson.Gson

object GsonUtil {

    private val _gson by lazy { Gson() }

    val gson = _gson
}