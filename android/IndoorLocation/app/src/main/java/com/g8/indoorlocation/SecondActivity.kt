package com.g8.indoorlocation

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener

class SecondActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_second)

        var databaseBeacon = FirebaseDatabase.getInstance().getReference("beacon")
        var databaseTest = FirebaseDatabase.getInstance().getReference("Test")
        var databasePosition = FirebaseDatabase.getInstance().getReference("position")

        var getData = object:ValueEventListener{
            override fun onCancelled(p0: DatabaseError) {
            }
            override fun onDataChange(p0: DataSnapshot) {
                var sb = StringBuilder()
                //get beacon
                var dataref =p0.child("1").child("signal")
                for (i in dataref.children) {
                    var rssi = i.child("rssi").getValue()
                    sb.append("$rssi")
                }
                Toast.makeText(this@SecondActivity, "$sb", Toast.LENGTH_LONG).show()
                //var rssiVal=findViewById(R.id.rssi1) as TextView
                //rssiVal.text=sb.toString()
            }
        }

        databaseBeacon.addValueEventListener(getData)
        databaseBeacon.addListenerForSingleValueEvent(getData)
        //databaseTest.addValueEventListener(getData)
        //databaseTest.addListenerForSingleValueEvent(getData)

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
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }

    }
    


}