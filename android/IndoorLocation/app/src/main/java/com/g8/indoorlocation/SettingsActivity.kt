package com.g8.indoorlocation

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

class SettingsActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_settings)


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
                    break
                }
                var rssiVal=findViewById(R.id.rssi1) as TextView
                rssiVal.text=sb.toString()

                sb.delete(0,sb.length)
                dataref =p0.child("2").child("signal")
                for (i in dataref.children) {
                    var rssi = i.child("rssi").getValue()
                    sb.append("$rssi")
                    break
                }
                rssiVal=findViewById(R.id.rssi2) as TextView
                rssiVal.text=sb.toString()
            }
        }

        databaseBeacon.addValueEventListener(getData)
        databaseBeacon.addListenerForSingleValueEvent(getData)
        //databaseTest.addValueEventListener(getData)
        //databaseTest.addListenerForSingleValueEvent(getData)

         
    }



    


}