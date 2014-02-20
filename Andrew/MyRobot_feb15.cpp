#include "WPILib.h"
#include "Math.h"
#include "Victor.h"
#include "Ultrasonic.h"
#include "Servo.h"
//#include "Compressor.h"
//#include "Solenoid.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 

class ByronMecanum : public IterativeRobot
{
	RobotDrive robotDrive; // robot drive system
	Victor shooter_1;  //shooter 1 V5
	Victor shooter_2;  //shooter 2 V6
	Victor aquire;  //Picker upper V9
	Victor lift_1; //Lifter 1 V7 
	Joystick stick_1; // driving
	Joystick stick_2; // shooting

	Gyro gyro; 
	Ultrasonic ultra;	
	
	float speed;
	
//	Compressor *m_compressor; 
//	bool solenoid_1;
//	bool solenoid_2;
		

public:
	ByronMecanum():
		robotDrive(1, 3, 2, 4),	// these must be initialized in the same order
		shooter_1(5),
		shooter_2(6),
		aquire(9),
		lift_1(7),
		stick_1(1),		// as they are declared above.	
		stick_2(2),		// same 
		
		gyro(1),
	    ultra(2, 2)
	{
		
		robotDrive.SetExpiration(0.1);
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
		
		robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor, true);
		robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor, true);
		
		speed = 0.0;
		//Compressor *c = new Compressor(4, 2);  c->Start(); 
		//solenoid_1 = false;
		//solenoid_2 = false;

	}
	
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 * Image processing code to identify 2013 Vision targets
 */
	
void ByronMecanum::RobotInit() {
	//Compressor *c = new Compressor(4, 2);  c->Start(); 
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void ByronMecanum::DisabledInit() {
	gyro.Reset();
	//delete m_compressor;

}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void ByronMecanum::DisabledPeriodic() {
	//delete m_compressor;


}

/**
 * Initialization code for autonomous mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters autonomous mode.
 */
void ByronMecanum::AutonomousInit() 

{
	gyro.Reset();
		
}

/**
 * Periodic code for autonomous mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in autonomous mode.
 */
void ByronMecanum::AutonomousPeriodic() 

{

}	
/**
 * Initialization code for teleop mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters teleop mode.
 */
void ByronMecanum::TeleopInit() {
	SmartDashboard::init ();
	
	gyro.Reset();
	ultra.SetAutomaticMode(true);

}
    

/**
 * Periodic code for teleop mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in teleop mode.
 */
void ByronMecanum::TeleopPeriodic() {

	double x = stick_1.GetX();          //variable for side to side movement
	double y = stick_1.GetY();          //variable for forward/backward movement
	double z = stick_1.GetZ()/2.3;      //varialbe for rotation	
	double deadzone = 0.28;				//variable for amount of deadzone
/*if (sqrt(pow(x, 2.0) + pow(y, 2.0)) < deadzone){
		gyro.Reset();
	}*/
	x = ((fabs(x) < deadzone) ? (0) : (x));
	y = ((fabs(y) < deadzone) ? (0) : (y));	
	z = ((fabs(z) < deadzone) ? (0) : (z));


	if (stick_2.GetRawButton(2)) {
		shooter_2.Set(stick_2.GetTwist()/0.5);
		aquire.Set(stick_2.GetTwist());
	}	else {
		shooter_2.Set(0.0);
		aquire.Set(0.0);
	}	
		
	if(stick_2.GetTrigger()){
		shooter_1.Set(stick_2.GetY());
		shooter_2.Set(stick_2.GetY());
	}	else {
		shooter_1.Set(0.0);
		shooter_2.Set(0.0);
	}
	 
	if (stick_2.GetRawButton(4)) {
		aquire.Set(speed);
		Wait(2.75);
		aquire.Set(0.0);
	}	
	if (stick_2.GetRawButton(5)) {
		aquire.Set(-speed);
		Wait(2.75);
		aquire.Set(0.0);
	}
	
	//solenoid_1 = stick_1.GetRawButton(3);
	//solenoid_2 = stick_1.GetRawButton(4);

	SmartDashboard::PutNumber("x",stick_1.GetX());
	SmartDashboard::PutNumber("y",stick_1.GetY());
	SmartDashboard::PutNumber("z",stick_1.GetZ());
	SmartDashboard::PutNumber("Twist", stick_2.GetTwist());
	SmartDashboard::PutNumber("Gyro",gyro.GetAngle());
	SmartDashboard::PutNumber("Range", ultra.GetRangeInches());
	speed = SmartDashboard::GetNumber("Speed");
	SmartDashboard::PutNumber("Speed", speed);	
	robotDrive.MecanumDrive_Cartesian(x, y, z);
	}
/**
 * Initialization code for test mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters test mode.
 */
void ByronMecanum::TestInit() {
}

/**
 * Periodic code for test mode should go here.
 *
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in test mode.
 */
void ByronMecanum::TestPeriodic() {
}

};

START_ROBOT_CLASS(ByronMecanum);