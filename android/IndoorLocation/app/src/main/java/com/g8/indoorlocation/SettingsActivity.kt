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
import kotlin.math.pow


class SettingsActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_settings)

        val MEASURED_RSSI = -55
        val PATH_LOSS_INDEX = 3

        var databaseBeacon = FirebaseDatabase.getInstance().getReference("beacon")
        var databaseTest = FirebaseDatabase.getInstance().getReference("Test")
        var databasePosition = FirebaseDatabase.getInstance().getReference("position")


        var getData = object:ValueEventListener{
            override fun onCancelled(p0: DatabaseError) {
            }
            override fun onDataChange(p0: DataSnapshot) {
                var sb = StringBuilder()
                var sbDistance = StringBuilder()
                //get beacon
                var dataref =p0.child("1").child("signal").child("rssi")
                var rssi = dataref.getValue()
                var distance = 10.0.pow((MEASURED_RSSI-rssi.toString().toInt()).toDouble()/(10*PATH_LOSS_INDEX))
                sb.append("$rssi")
                var xxxxx=(MEASURED_RSSI-rssi.toString().toInt()).toDouble()
                var yyyyy=10*PATH_LOSS_INDEX
                var zzzzz=xxxxx/yyyyy
                var ppppp=10.0.pow(zzzzz)
                //sbDistance.append("$zzzzz")
                //sbDistance.append(" / ")
                //sbDistance.append("$ppppp")

                sbDistance.append("$distance")

                var rssiVal=findViewById(R.id.rssi1) as TextView
                rssiVal.text=sb.toString()
                var distanceVal=findViewById(R.id.distance1) as TextView
                distanceVal.text=sbDistance.toString()


                sb.delete(0,sb.length)
                sbDistance.delete(0,sbDistance.length)
                dataref =p0.child("2").child("signal").child("rssi")
                rssi= dataref.getValue()
                distance = 10.0.pow((MEASURED_RSSI-rssi.toString().toInt()).toDouble()/(10*PATH_LOSS_INDEX))
                sb.append("$rssi")
                sbDistance.append("$distance")

                rssiVal=findViewById(R.id.rssi2) as TextView
                rssiVal.text=sb.toString()
                distanceVal=findViewById(R.id.distance2) as TextView
                distanceVal.text=sbDistance.toString()


                sb.delete(0,sb.length)
                sbDistance.delete(0,sbDistance.length)
                dataref =p0.child("3").child("signal").child("rssi")
                rssi = dataref.getValue()
                distance = 10.0.pow((MEASURED_RSSI-rssi.toString().toInt()).toDouble()/(10*PATH_LOSS_INDEX))
                sb.append("$rssi")
                sbDistance.append("$distance")

                rssiVal=findViewById(R.id.rssi3) as TextView
                rssiVal.text=sb.toString()
                distanceVal=findViewById(R.id.distance3) as TextView
                distanceVal.text=sbDistance.toString()
            }
        }

        databaseBeacon.addValueEventListener(getData)
        databaseBeacon.addListenerForSingleValueEvent(getData)
        //databaseTest.addValueEventListener(getData)
        //databaseTest.addListenerForSingleValueEvent(getData)

         
    }


    fun gototwo(v: View?) {
        if (v != null) {
            val intent = Intent(this, SecondActivity::class.java)
            startActivity(intent)}
    }
    


}