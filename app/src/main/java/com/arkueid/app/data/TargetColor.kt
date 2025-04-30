package com.arkueid.app.data

import android.os.Parcel
import android.os.Parcelable

data class Color(var r: Float, var g: Float, var b: Float) : Parcelable {
    constructor(parcel: Parcel) : this(
        parcel.readFloat(),
        parcel.readFloat(),
        parcel.readFloat()
    ) {
    }

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeFloat(r)
        parcel.writeFloat(g)
        parcel.writeFloat(b)
    }

    override fun describeContents(): Int {
        return 0
    }

    companion object CREATOR : Parcelable.Creator<Color> {
        override fun createFromParcel(parcel: Parcel): Color {
            return Color(parcel)
        }

        override fun newArray(size: Int): Array<Color?> {
            return arrayOfNulls(size)
        }
    }

}

data class TargetColor(
    var type: Int,
    var targetIndex: Int, // partId, drawableId
    var color: Color,
) : Parcelable {
    constructor(parcel: Parcel) : this(
        parcel.readInt(),
        parcel.readInt(),
        parcel.readTypedObject(Color.CREATOR)!!
    )

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeInt(type)
        parcel.writeInt(targetIndex)
        parcel.writeTypedObject(color, 0)
    }

    override fun describeContents(): Int = 0

    companion object CREATOR : Parcelable.Creator<TargetColor> {
        override fun createFromParcel(parcel: Parcel): TargetColor {
            return TargetColor(parcel)
        }

        override fun newArray(size: Int): Array<TargetColor?> {
            return arrayOfNulls(size)
        }
    }

    val isPart = type == 0
}
