package com.g8.indoorlocation

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.text.TextUtils
import android.util.Patterns
import android.view.View
import android.widget.Toast
import com.g8.indoorlocation.databinding.ActivityMainBinding
import com.google.firebase.auth.FirebaseAuth
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener
import java.lang.StringBuilder


class MainActivity : AppCompatActivity() {


    private  lateinit var binding: ActivityMainBinding
    private lateinit var firebaseAuth: FirebaseAuth
    private var email = ""
    private var password = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        //setContentView(R.layout.activity_main)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        firebaseAuth = FirebaseAuth.getInstance()
        checkUser()

        binding.buttonlogin.setOnClickListener{
            validateData()
        }
    }

    fun gototwo(v: View?) {
        if (v != null) {
            val intent = Intent(this, SecondActivity::class.java)
            startActivity(intent)
        }
    }

    private fun checkUser() {
        val firebaseUser = firebaseAuth.currentUser
        if(firebaseUser != null){
            //user is already log
            val intent = Intent(this, SecondActivity::class.java)
            startActivity(intent)
            super.onBackPressed()
            //finish()
        }
    }

    private fun validateData() {
        email = binding.emailtxt.text.toString().trim()
        password = binding.passwordtxt.text.toString().trim()

        if (!Patterns.EMAIL_ADDRESS.matcher(email).matches()) {
            binding.emailtxt.error = "Wrong Email Adress"
        }
        else if (TextUtils.isEmpty(password)) {
            binding.passwordtxt.error = "Password is empty"

        } else {
            firebaseAuth.signInWithEmailAndPassword(email, password)
                .addOnSuccessListener {
                    startActivity(Intent(this, SecondActivity::class.java))
                    finish()
                }
                .addOnFailureListener { e->
                    Toast.makeText(this, "Login failed due to ${e.message}", Toast.LENGTH_SHORT)
                }
        }

    }



}