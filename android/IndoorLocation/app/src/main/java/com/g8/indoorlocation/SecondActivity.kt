package com.g8.indoorlocation

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.graphics.BitmapFactory
import android.graphics.Color
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.*
import androidx.fragment.app.Fragment
import com.google.android.material.floatingactionbutton.FloatingActionButton
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import kotlin.math.pow

class SecondActivity : AppCompatActivity() {

    private lateinit var firebaseAuth: FirebaseAuth
    lateinit var notificationManager: NotificationManager
    lateinit var notificationChannel: NotificationChannel
    lateinit var builder: Notification.Builder
    private val channelId = "com.g8.indoorlocation"
    private var beaconNum=0


    override fun onCreate(savedInstanceState: Bundle?) {
        firebaseAuth = FirebaseAuth.getInstance()
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_second)

        val MEASURED_RSSI = -55

        var databaseBeacon = FirebaseDatabase.getInstance().getReference("beacon")


        var getData = object:ValueEventListener{
            override fun onCancelled(p0: DatabaseError) {
            }
            override fun onDataChange(p0: DataSnapshot) {

                notificationManager = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
                //get beacon
                var dataref =p0.child("1").child("signal").child("rssi")
                var rssi = dataref.getValue().toString().toInt()

                var bcn1=findViewById(R.id.bcn1) as ImageView
                var bcn1green=findViewById(R.id.bcn1green) as ImageView

                if(rssi>=MEASURED_RSSI){
                    bcn1green.visibility=View.VISIBLE
                    bcn1.visibility=View.INVISIBLE
                    beaconNum=1
                }
                else{
                    bcn1green.visibility=View.INVISIBLE
                    bcn1.visibility=View.VISIBLE
                }




                dataref =p0.child("2").child("signal").child("rssi")
                rssi= dataref.getValue().toString().toInt()

                var bcn2=findViewById(R.id.bcn2) as ImageView
                var bcn2green=findViewById(R.id.bcn2green) as ImageView

                if(rssi>=MEASURED_RSSI){
                    bcn2green.visibility=View.VISIBLE
                    bcn2.visibility=View.INVISIBLE
                    beaconNum=2
                }
                else{
                    bcn2green.visibility=View.INVISIBLE
                    bcn2.visibility=View.VISIBLE
                }


                dataref =p0.child("3").child("signal").child("rssi")
                rssi = dataref.getValue().toString().toInt()

                var bcn3=findViewById(R.id.bcn3) as ImageView
                var bcn3green=findViewById(R.id.bcn3green) as ImageView

                if(rssi>=MEASURED_RSSI){
                    bcn3green.visibility=View.VISIBLE
                    bcn3.visibility=View.INVISIBLE
                    val intent = Intent(this@SecondActivity, SecondActivity::class.java)
                    val pendingIntent = PendingIntent.getActivity(this@SecondActivity, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT)
                    //val contentView = RemoteViews(packageName, R.layout.SecondActivity)


                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                        notificationChannel = NotificationChannel(channelId, "You arrived at beacon 3!", NotificationManager.IMPORTANCE_HIGH)
                        notificationChannel.enableLights(true)
                        notificationChannel.lightColor = Color.GREEN
                        notificationChannel.enableVibration(true)
                        notificationManager.createNotificationChannel(notificationChannel)

                        builder = Notification.Builder(this@SecondActivity, channelId)
                            .setSmallIcon(R.drawable.icon)
                            //.setLargeIcon(BitmapFactory.decodeResource(this.resources, R.drawable.ic_launcher_background))
                            .setContentIntent(pendingIntent)
                            .setContentText("You arrived at beacon 3!")
                            .setContentTitle("Indoor Location")
                    } else {

                        builder = Notification.Builder(this@SecondActivity)
                            //.setContent(contentView)
                            .setSmallIcon(R.drawable.icon)
                            //.setLargeIcon(BitmapFactory.decodeResource(this.resources, R.drawable.ic_launcher_background))
                            .setContentIntent(pendingIntent)
                            .setContentText("You arrived at beacon 3!")
                            .setContentTitle("Indoor Location")
                    }
                    notificationManager.notify(3, builder.build())



                }
                else{
                    bcn3green.visibility=View.INVISIBLE
                    bcn3.visibility=View.VISIBLE
                }

            }
        }

        databaseBeacon.addValueEventListener(getData)
        databaseBeacon.addListenerForSingleValueEvent(getData)



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
            firebaseAuth.signOut()
            startActivity(Intent(this, MainActivity::class.java))
        }
    }

    fun openset(v: View?) {
        if (v != null) {
            val intent = Intent(this, SettingsActivity::class.java)
            startActivity(intent)
        }

    }
    


}