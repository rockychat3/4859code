#include "WPILib.h"
#include "math.h"
#include "Compressor.h"
#include "Solenoid.h"
#include "Victor.h"
#include "Ultrasonic.h"
#include "Servo.h"


/**
 * This is a demo program showing the use of the RobotBase class.
 * The IterativeRobot class is the base of a robot application that will automatically call your
 * Periodic methods for each packet based on the mode.
 */ 

class ByronMecanum : public IterativeRobot
{
	RobotDrive robotDrive; // robot drive system
	Victor v5;  //shooter 1
	Victor v6;  //shooter 2
	Victor v7;  //Picker upper
	Joystick stick_1; // driving
	Joystick stick_2; // shooting
	Gyro gyro; 
	Ultrasonic ultra;	
	Servo servo_1;
	
	Compressor *m_compressor; 
	bool solenoid_1;
	bool solenoid_2;
	
	


public:
	ByronMecanum():
		robotDrive(1, 3, 2, 4),	// these must be initialized in the same order
		v5(5),
		v6(6),
		v7(7),
		stick_1(1),		// as they are declared above.	
		stick_2(2),		// same 
		gyro(1),
	    ultra(2, 2),
	    servo_1(10)
	{
		
		robotDrive.SetExpiration(0.1);
		robotDrive.SetInvertedMotor(robotDrive.kFrontRightMotor, true);
		robotDrive.SetInvertedMotor(robotDrive.kRearRightMotor, true);
		
		this->SetPeriod(0); 	//Set update period to sync with robot control packets (20ms nominal)
		
		Compressor *c = new Compressor(4, 2);  c->Start(); 
		solenoid_1 = false;
		solenoid_2 = false;
	}
	
/**
 * Robot-wide initialization code should go here.
 * 
 * Use this method for default Robot-wide initialization which will
 * be called when the robot is first powered on.  It will be called exactly 1 time.
 */
void ByronMecanum::RobotInit() {
	Compressor *c = new Compressor(4, 2);  c->Start(); 
	 // Actions which would be performed once (and only once) upon initialization of the
	// robot would be put here.
}

/**
 * Initialization code for disabled mode should go here.
 * 
 * Use this method for initialization code which will be called each time
 * the robot enters disabled mode. 
 */
void ByronMecanum::DisabledInit() {
	gyro.Reset();
	delete m_compressor;

}

/**
 * Periodic code for disabled mode should go here.
 * 
 * Use this method for code which will be called periodically at a regular
 * rate while the robot is in disabled mode.
 */
void ByronMecanum::DisabledPeriodic() {
	delete m_compressor;


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

	gyro.SetSensitivity(.005);   //Setting its sensitivity in volts per second
	float angle = gyro.GetAngle();   //get heading
	robotDrive.Drive (-1.0, -angle / 30.0);  //turn to correct heading
	Wait (0.004);
	
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
	
	solenoid_1 = stick_1.GetRawButton(3);
	solenoid_2 = stick_1.GetRawButton(4);
	if (stick_2.GetRawButton(5)) {
		servo_1.Set(0.5);
		servo_1.SetAngle(75);
	}
	else {
		servo_1.Set(0.0);
		servo_1.SetAngle(0);
	}
		
	
	//v1.SetSpeed(0.5);
	//v2.SetSpeed(0.5);
	//v3.SetSpeed(0.5);
	//v4.SetSpeed(0.5);
	//v5.Set(0.0);
	//v6.Set(0.0);  
	/*if (stick_1.GetTrigger()){
		v5.Set(-1.0);
		v6.Set(-1.0);
	}*/
	
	if (stick_2.GetRawButton(2)) {
		v7.Set(-1.0);
	}
	else {	
		v7.Set(0.0);
	}
	if (stick_2.GetRawButton(4)) {
		v7.Set(1.0);
		v6.Set(-0.5);
	}
		
	if(stick_2.GetTrigger()){
		v5.Set(stick_2.GetY());
		v6.Set(stick_2.GetY());
	}
	
	SmartDashboard::PutNumber("range", ultra.GetRangeInches());
	
	double x = stick_1.GetX();          //variable for side to side movement
	double y = stick_1.GetY();          //variable for forward/backward movement
	double z = stick_1.GetZ()/2.3;      //varialbe for rotation	
	double deadzone = 0.28;				//variable for amount of deadzone
	if (sqrt(pow(x, 2.0) + pow(y, 2.0)) < deadzone){
		gyro.Reset();
	}
	x = ((fabs(x) < deadzone) ? (0) : (x));
	y = ((fabs(y) < deadzone) ? (0) : (y));	
	z = ((fabs(z) < deadzone) ? (0) : (z));	
	SmartDashboard::PutNumber("gyro",gyro.GetAngle());
	SmartDashboard::PutNumber("x",stick_1.GetX());
	SmartDashboard::PutNumber("y",stick_1.GetY());
	SmartDashboard::PutNumber("z",stick_1.GetZ());
	robotDrive.MecanumDrive_Cartesian(x, y, z, gyro.GetAngle());

	
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