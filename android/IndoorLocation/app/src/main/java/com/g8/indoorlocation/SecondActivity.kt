package com.g8.indoorlocation

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.google.android.material.floatingactionbutton.FloatingActionButton

class SecondActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_second)

    }


    private var clicked= false


    fun showmore(v: View?) {
        if (v != null) {
            setVisibility(clicked)
            setAnimation(clicked)
            clicked = !clicked
        }

    }
    private fun setVisibility(clicked: Boolean){
        if(!clicked){
            val idperson = findViewById(R.id.idperson) as com.google.android.material.floatingactionbutton.FloatingActionButton
            val settings = findViewById(R.id.settings) as com.google.android.material.floatingactionbutton.FloatingActionButton
            settings.visibility=View.VISIBLE
            idperson.visibility=View.VISIBLE
            idperson.isClickable= true
            settings.isClickable= true
        }else{
            val idperson = findViewById(R.id.idperson) as com.google.android.material.floatingactionbutton.FloatingActionButton
            val settings = findViewById(R.id.settings) as com.google.android.material.floatingactionbutton.FloatingActionButton
            settings.visibility=View.INVISIBLE
            idperson.visibility=View.INVISIBLE
            idperson.isClickable= false
            settings.isClickable= false
        }
    }

    private fun setAnimation(clicked: Boolean){
        if(!clicked){
           val idperson = findViewById(R.id.idperson) as com.google.android.material.floatingactionbutton.FloatingActionButton
           val settings = findViewById(R.id.settings) as com.google.android.material.floatingactionbutton.FloatingActionButton
           val more = findViewById(R.id.more) as com.google.android.material.floatingactionbutton.FloatingActionButton
           val rotateOp = AnimationUtils.loadAnimation(this,R.anim.rotate_open_anim)
           val fromBot = AnimationUtils.loadAnimation(this,R.anim.from_bottom_anim)
           idperson.startAnimation(fromBot)
           settings.startAnimation(fromBot)
           more.startAnimation(rotateOp)
           idperson.isClickable= true
           settings.isClickable= true

        }else{
           val rotateCl = AnimationUtils.loadAnimation(this,R.anim.rotate_close_anim)
           val toBot = AnimationUtils.loadAnimation(this,R.anim.to_bottom_anim)
           val idperson = findViewById(R.id.idperson) as com.google.android.material.floatingactionbutton.FloatingActionButton
           val settings = findViewById(R.id.settings) as com.google.android.material.floatingactionbutton.FloatingActionButton
           val more = findViewById(R.id.more) as com.google.android.material.floatingactionbutton.FloatingActionButton
           idperson.startAnimation(toBot)
           settings.startAnimation(toBot)
           more.startAnimation(rotateCl)
           idperson.isClickable= false
           settings.isClickable= false
        }
    }

    fun showid(v: View?) {
        if (v != null) {
            Toast.makeText(this, "show id here!", Toast.LENGTH_SHORT).show()
        }
    }

    fun openset(v: View?) {
        if (v != null) {
            Toast.makeText(this, "Not done yet ;3 !", Toast.LENGTH_SHORT).show()
        }

    }
    


}