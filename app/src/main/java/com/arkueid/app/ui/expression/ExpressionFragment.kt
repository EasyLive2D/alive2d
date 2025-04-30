package com.arkueid.app.ui.expression

import android.os.Bundle
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.arkueid.app.R

/**
 * A fragment representing a list of Items.
 */
class ExpressionFragment : Fragment() {

    private lateinit var expressions: List<Expression>

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val view = inflater.inflate(R.layout.fragment_expression_list, container, false)

        if (view is RecyclerView) {
            with(view) {
                layoutManager = LinearLayoutManager(context)
                adapter = ExpressionRecyclerViewAdapter(expressions)
            }
        }
        return view
    }

    companion object {

        @JvmStatic
        fun newInstance(expressions: List<Expression>) =
            ExpressionFragment().apply {
                this.expressions = expressions
            }
    }
}