package com.arkueid.app.ui

import androidx.fragment.app.FragmentActivity
import androidx.viewpager2.adapter.FragmentStateAdapter
import com.arkueid.app.data.ModelInfo
import com.arkueid.app.ui.expression.Expression
import com.arkueid.app.ui.expression.ExpressionFragment

class PageAdapter(modelInfo: ModelInfo, activity: FragmentActivity) : FragmentStateAdapter(activity) {

    private val expressions: List<Expression> = modelInfo.model.expressions.map {
        val name = it["Name"]!!
        Expression(modelInfo, name, modelInfo.preference.activeExpressions.contains(name))
    }

    override fun getItemCount(): Int = 1

    override fun createFragment(position: Int) = when(position) {
        0 -> ExpressionFragment.newInstance(expressions)
        else -> throw RuntimeException()
    }

}