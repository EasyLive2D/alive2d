package com.arkueid.app.ui.expression

import android.view.LayoutInflater
import android.view.ViewGroup
import android.widget.Switch
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.arkueid.app.databinding.FragmentExpressionBinding

class ExpressionRecyclerViewAdapter(
    private val values: List<Expression>
) : RecyclerView.Adapter<ExpressionRecyclerViewAdapter.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {

        return ViewHolder(
            FragmentExpressionBinding.inflate(
                LayoutInflater.from(parent.context),
                parent,
                false
            )
        )

    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        val item = values[position]
        holder.name.text = item.name

        holder.switch.setOnCheckedChangeListener(null)
        holder.switch.isChecked = item.active
        holder.switch.setOnCheckedChangeListener { _, isChecked ->
            item.active = isChecked
        }
    }

    override fun getItemCount(): Int = values.size

    inner class ViewHolder(binding: FragmentExpressionBinding) :
        RecyclerView.ViewHolder(binding.root) {
        val name: TextView = binding.name
        val switch: Switch = binding.active
    }

}