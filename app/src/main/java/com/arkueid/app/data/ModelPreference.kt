package com.arkueid.app.data

import android.os.Parcel
import android.os.Parcelable


data class ModelPreference(
    var lipSyncN: Float = 2f,
    var offsetX: Float = 0.0f,
    var offsetY: Float = 0.0f,
    var scale: Float = 1.0f,
    // expressionId -> list[ColorScheme]
    var colorSchemes: MutableMap<String, MutableList<TargetColor>> = mutableMapOf(),
    // list[expressionId]
    var activeExpressions: MutableList<String> = mutableListOf(),
) : Parcelable {
    constructor(parcel: Parcel) : this(
        parcel.readFloat(),
        parcel.readFloat(),
        parcel.readFloat(),
        parcel.readFloat(),
        mutableMapOf(),
        mutableListOf()
    ) {
        val size = parcel.readInt()
        repeat(size) {
            val key = parcel.readString()!!
            val list = mutableListOf<TargetColor>()
            parcel.readTypedList(list, TargetColor.CREATOR)
            colorSchemes[key] = list
        }
        parcel.readStringList(activeExpressions)
    }

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeFloat(lipSyncN)
        parcel.writeFloat(offsetX)
        parcel.writeFloat(offsetY)
        parcel.writeFloat(scale)
        parcel.writeInt(colorSchemes.size)
        colorSchemes.keys.forEach { key ->
            parcel.writeString(key)
            parcel.writeTypedList(colorSchemes[key])
        }
        parcel.writeStringList(activeExpressions)
    }

    override fun describeContents(): Int {
        return 0
    }

    companion object CREATOR : Parcelable.Creator<ModelPreference> {
        override fun createFromParcel(parcel: Parcel): ModelPreference {
            return ModelPreference(parcel)
        }

        override fun newArray(size: Int): Array<ModelPreference?> {
            return arrayOfNulls(size)
        }
    }

}